DROP SCHEMA IF EXISTS storage CASCADE;

CREATE SCHEMA storage;

CREATE TABLE bucket (
    -- The bucket's unique internal identifier.
    id              uuid PRIMARY KEY,

    -- The bucket's user-assigned name.
    name            varchar(128) UNIQUE NOT NULL,

    -- The time this bucket was first created.
    date_created    timestamptz DEFAULT NOW()
);

CREATE TABLE object (
    -- The object's unique identifier.
    id              uuid PRIMARY KEY,

    -- SHA256 checksum of the object's contents.
    hash            char(64) UNIQUE NOT NULL,

    -- Size of the object's contents in bytes.
    size            bigint NOT NULL,

    -- Mime type of the object's contents.
    mime_type       varchar(50) NOT NULL,

    -- The time this object was first added to the object store.
    date_added      timestamptz DEFAULT NOW()
);

CREATE TABLE bucket_object (
    bucket_id       uuid REFERENCES bucket(id) ON DELETE CASCADE,
    object_id       uuid REFERENCES object(id) ON DELETE CASCADE,
    date_added      timestamptz DEFAULT NOW(),

    PRIMARY KEY (bucket_id, object_id)
);

CREATE VIEW bucket_view AS
SELECT
    bucket.id,
    name,
    date_created,
    count(object_id) AS object_count,
    coalesce(sum(size), 0) AS space_used
FROM bucket
    LEFT OUTER JOIN bucket_object
        ON bucket_id = bucket.id
    LEFT OUTER JOIN object
        ON object_id = object.id
GROUP BY bucket.id, name;

CREATE VIEW bucket_contents AS
SELECT
    bucket_id,
    object_id,
    hash,
    size,
    mime_type,
    bo.date_added
FROM bucket_object bo
    JOIN object
        ON bo.object_id = object.id;

CREATE VIEW object_reference AS
SELECT
    id AS object_id,
    count(object_id) AS reference_count
FROM object
    LEFT OUTER JOIN bucket_object
        ON bucket_object.object_id = object.id
GROUP BY id;

CREATE FUNCTION create_object(
    object_id          uuid,
    object_hash        char(64),
    object_size        bigint,
    object_mime_type   varchar
) RETURNS uuid AS $$
DECLARE
    id_for_hash     uuid;
BEGIN
    INSERT INTO object (
        id,
        hash,
        size,
        mime_type
    ) VALUES (
        object_id,
        object_hash,
        object_size,
        object_mime_type
    ) ON CONFLICT DO NOTHING;

    SELECT id INTO id_for_hash
    FROM object
    WHERE hash = object_hash;

    RETURN id_for_hash;
END;
$$ LANGUAGE plpgsql;

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
