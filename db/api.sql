CREATE FUNCTION add_object(
    a_bucket_id uuid,
    a_object_id uuid,
    a_hash      varchar,
    a_size      bigint,
    a_mime_type varchar
) RETURNS SETOF object AS $$
BEGIN
    INSERT INTO bucket_object (
        bucket_id,
        object_id
    ) VALUES (
        a_bucket_id,
        (
            SELECT create_object(
                a_object_id,
                a_hash,
                a_size,
                a_mime_type
            )
        )
    ) ON CONFLICT DO NOTHING;

    RETURN QUERY
    SELECT
        object_id AS id,
        hash,
        size,
        mime_type,
        date_added
    FROM bucket_contents
    WHERE bucket_id = a_bucket_id
        AND hash = a_hash;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_bucket(
    bucket_id       uuid,
    bucket_name     varchar
) RETURNS SETOF bucket AS $$
BEGIN
    RETURN QUERY
    INSERT INTO bucket (
        id,
        name
    ) VALUES (
        bucket_id,
        bucket_name
    ) RETURNING *;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION fetch_bucket(
    bucket_name     varchar
) RETURNS SETOF bucket_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM bucket_view
    WHERE name = bucket_name;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION fetch_buckets()
RETURNS SETOF bucket_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM bucket_view;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION fetch_buckets(
    names   varchar[]
) RETURNS SETOF bucket_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM bucket_view
    WHERE name = ANY(names);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION fetch_store_totals()
RETURNS TABLE (
    buckets     bigint,
    objects     bigint,
    space_used  numeric
) AS $$
BEGIN
    RETURN QUERY
    SELECT
        (SELECT count(*) FROM bucket)
            AS buckets,
        (SELECT count(*) FROM object)
            AS objects,
        (SELECT COALESCE(sum(size), 0) FROM object)
            AS space_used;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION get_object(
    a_bucket_id uuid,
    a_object_id uuid
) RETURNS SETOF object AS $$
BEGIN
    RETURN QUERY
    SELECT
        object_id AS id,
        hash,
        size,
        mime_type,
        date_added
    FROM bucket_contents
    WHERE bucket_id = a_bucket_id
        AND object_id = a_object_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION remove_bucket(
    bucket_id uuid
) RETURNS void AS $$
BEGIN
    DELETE FROM bucket
    WHERE id = bucket_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION remove_object(
    a_bucket_id uuid,
    a_object_id uuid
) RETURNS SETOF object AS $$
BEGIN
    RETURN QUERY
    WITH deleted AS (
        DELETE FROM bucket_object
        WHERE bucket_id = a_bucket_id AND object_id = a_object_id
        RETURNING object_id, date_added
    )
    SELECT
        object_id,
        hash,
        size,
        mime_type,
        deleted.date_added
    FROM deleted
        JOIN object ON object_id = object.id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION remove_orphan_objects()
RETURNS SETOF object AS $$
BEGIN
    RETURN QUERY
    DELETE FROM object USING object_reference
    WHERE id = object_id AND reference_count = 0
    RETURNING id, hash, size, mime_type, date_added;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION rename_bucket(
    bucket_id       uuid,
    bucket_name     varchar(128)
) RETURNS void AS $$
BEGIN
    UPDATE bucket
    SET name = bucket_name
    WHERE id = bucket_id;
END;
$$ LANGUAGE plpgsql;
