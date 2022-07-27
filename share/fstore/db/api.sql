DROP SCHEMA IF EXISTS fstore CASCADE;
CREATE SCHEMA fstore;

SET search_path TO fstore;

CREATE FUNCTION epoch_sec(
    a_timestamp     timestamptz
) RETURNS numeric AS $$
BEGIN
    RETURN extract(epoch FROM a_timestamp);
END;
$$ LANGUAGE plpgsql;

CREATE VIEW bucket_view AS
SELECT
    bucket_id,
    name,
    epoch_sec(date_created) AS date_created,
    count(object_id) AS object_count,
    coalesce(sum(size), 0) AS space_used
FROM data.bucket
LEFT JOIN data.bucket_object USING (bucket_id)
LEFT JOIN data.object USING (object_id)
GROUP BY bucket_id, name;

CREATE VIEW bucket_contents AS
SELECT
    bucket_id,
    object_id,
    hash,
    size,
    "type",
    subtype,
    epoch_sec(bucket_object.date_added) AS date_added
FROM data.bucket_object
JOIN data.object USING (object_id);

CREATE VIEW object AS
SELECT
    object_id,
    hash,
    size,
    "type",
    subtype,
    epoch_sec(date_added) AS date_added
FROM data.object;

CREATE VIEW object_error AS
SELECT
    object_id,
    message
FROM data.object_error;

CREATE VIEW object_ref AS
SELECT
    object_id,
    count(bucket_objects.object_id) AS reference_count
FROM data.object
LEFT JOIN data.bucket_object bucket_objects USING (object_id)
GROUP BY object_id;

CREATE TYPE remove_result AS (
    objects_removed bigint,
    space_freed     bigint
);

CREATE TYPE store_totals AS (
    buckets         bigint,
    objects         bigint,
    space_used      bigint
);

CREATE FUNCTION clear_error(
    a_object_id     uuid
) RETURNS void AS $$
BEGIN
    DELETE FROM data.object_error
    WHERE object_id = a_object_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_object(
    a_object_id     uuid,
    a_hash          text,
    a_size          bigint,
    a_type          text,
    a_subtype       text
) RETURNS uuid AS $$
DECLARE
    id_for_hash     uuid;
BEGIN
    INSERT INTO data.object (
        object_id,
        hash,
        size,
        "type",
        subtype
    ) VALUES (
        a_object_id,
        a_hash,
        a_size,
        a_type,
        a_subtype
    ) ON CONFLICT DO NOTHING;

    SELECT object_id INTO id_for_hash
    FROM data.object
    WHERE hash = a_hash;

    RETURN id_for_hash;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION add_error(
    a_object_id     uuid,
    a_message       text
) RETURNS void AS $$
BEGIN
    INSERT INTO data.object_error (
        object_id,
        message
    ) VALUES (
        a_object_id,
        a_message
    ) ON CONFLICT (object_id)
    DO UPDATE SET message = a_message;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION add_object(
    a_bucket_id     uuid,
    a_object_id     uuid,
    a_hash          text,
    a_size          bigint,
    a_type          text,
    a_subtype       text
) RETURNS SETOF object AS $$
BEGIN
    INSERT INTO data.bucket_object (
        bucket_id,
        object_id
    ) VALUES (
        a_bucket_id,
        (
            SELECT create_object(
                a_object_id,
                a_hash,
                a_size,
                a_type,
                a_subtype
            )
        )
    ) ON CONFLICT DO NOTHING;

    RETURN QUERY
    SELECT
        object_id,
        hash,
        size,
        "type",
        subtype,
        date_added
    FROM bucket_contents
    WHERE bucket_id = a_bucket_id AND hash = a_hash;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_bucket(
    a_bucket_id     uuid,
    a_name          text
) RETURNS SETOF bucket_view AS $$
BEGIN
    INSERT INTO data.bucket (
        bucket_id,
        name
    ) VALUES (
        a_bucket_id,
        a_name
    );

    RETURN QUERY
    SELECT * FROM fetch_bucket(a_name);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION fetch_bucket(
    a_name          text
) RETURNS SETOF bucket_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM bucket_view
    WHERE name = a_name;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION fetch_buckets(
    a_names         text[]
) RETURNS SETOF bucket_view AS $$
BEGIN
    RETURN QUERY
    WITH names AS (
        SELECT
            ordinality,
            unnest AS name
        FROM unnest(a_names) WITH ordinality
    )
    SELECT
        bucket_id,
        name,
        date_created,
        object_count,
        space_used
    FROM names
    JOIN bucket_view USING (name)
    ORDER BY ordinality;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION fetch_buckets_all()
RETURNS SETOF bucket_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM bucket_view;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION fetch_store_totals()
RETURNS SETOF store_totals AS $$
BEGIN
    RETURN QUERY
    SELECT
        (SELECT count(*) FROM data.bucket) AS buckets,
        (SELECT count(*) FROM data.object) AS objects,
        (SELECT coalesce(sum(size)::bigint, 0) FROM data.object) AS space_used;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION get_errors()
RETURNS SETOF object_error AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM object_error;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION get_object(
    a_bucket_id     uuid,
    a_object_id     uuid
) RETURNS SETOF object AS $$
BEGIN
    RETURN QUERY
    SELECT
        object_id,
        hash,
        size,
        "type",
        subtype,
        date_added
    FROM bucket_contents
    WHERE bucket_id = a_bucket_id AND object_id = a_object_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION get_objects() RETURNS refcursor AS $$
DECLARE
    ref refcursor;
BEGIN
    OPEN ref FOR
    SELECT *
    FROM object
    ORDER BY object_id;

    RETURN ref;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION remove_bucket(
    a_bucket_id     uuid
) RETURNS void AS $$
BEGIN
    DELETE FROM data.bucket
    WHERE bucket_id = a_bucket_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION remove_object(
    a_bucket_id     uuid,
    a_object_id     uuid
) RETURNS SETOF object AS $$
BEGIN
    RETURN QUERY
    WITH deleted AS (
        DELETE FROM data.bucket_object
        WHERE bucket_id = a_bucket_id AND object_id = a_object_id
        RETURNING object_id, date_added
    )
    SELECT
        object_id,
        hash,
        size,
        "type",
        subtype,
        epoch_sec(deleted.date_added) AS date_added
    FROM deleted
    JOIN data.object USING (object_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION remove_objects(
    a_bucket_id     uuid,
    a_objects       uuid[]
) RETURNS SETOF remove_result AS $$
BEGIN
    RETURN QUERY
    WITH deleted AS (
        DELETE FROM data.bucket_object
        WHERE bucket_id = a_bucket_id AND object_id = ANY(a_objects)
        RETURNING object_id
    )
    SELECT
        count(*) AS objects_removed,
        coalesce(sum(size)::bigint, 0) AS space_freed
    FROM deleted
    JOIN data.object USING (object_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION remove_orphan_objects()
RETURNS SETOF object AS $$
BEGIN
    RETURN QUERY
    DELETE FROM data.object obj USING object_ref ref
    WHERE obj.object_id = ref.object_id AND reference_count = 0
    RETURNING
        obj.object_id,
        hash,
        size,
        "type",
        subtype,
        epoch_sec(date_added) AS date_added;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION rename_bucket(
    a_bucket_id     uuid,
    a_bucket_name   text
) RETURNS void AS $$
BEGIN
    UPDATE data.bucket
    SET name = a_bucket_name
    WHERE bucket_id = a_bucket_id;
END;
$$ LANGUAGE plpgsql;
