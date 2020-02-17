DROP SCHEMA IF EXISTS storage CASCADE;

CREATE SCHEMA storage

CREATE TABLE bucket (
    id              uuid PRIMARY KEY,
    name            varchar(128) UNIQUE NOT NULL CHECK (length(trim(name)) <> 0),
    date_created    timestamptz DEFAULT NOW()
)

CREATE TABLE object (
    -- The object's unique identifier.
    id              uuid PRIMARY KEY,

    -- SHA256 checksum of the object's contents.
    hash            char(64) UNIQUE NOT NULL,

    -- Length of the object's contents in bytes.
    len             bigint NOT NULL,

    -- Mime type of the object's contents.
    mime_type       varchar(50) NOT NULL,

    -- The time this object was first added to the object store.
    date_added      timestamptz DEFAULT NOW()
)

CREATE TABLE bucket_object (
    bucket_id       uuid REFERENCES bucket(id) ON DELETE CASCADE,
    object_id       uuid REFERENCES object(id) ON DELETE CASCADE,
    date_added      timestamptz DEFAULT NOW(),

    PRIMARY KEY (bucket_id, object_id)
)

CREATE VIEW object_reference AS
    SELECT
        id AS object_id,
        count(object_id) AS reference_count
    FROM object
        LEFT OUTER JOIN bucket_object
            ON bucket_object.object_id = object.id
    GROUP BY id

; -- END SCHEMA storage

CREATE FUNCTION storage.add_object(
    bucket_id       uuid,
    object_id       uuid
) RETURNS uuid AS $$
BEGIN
    INSERT INTO storage.bucket_object (
        bucket_id,
        object_id
    ) VALUES (
        bucket_id,
        object_id
    ) ON CONFLICT DO NOTHING;

    RETURN object_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION storage.create_bucket(
    bucket_id       uuid,
    bucket_name     varchar(128)
) RETURNS void AS $$
BEGIN
    INSERT INTO storage.bucket (
        id,
        name
    ) VALUES (
        bucket_id,
        bucket_name
    );
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION storage.create_object(
    object_id          uuid,
    object_hash        char(64),
    object_len         bigint,
    object_mime_type   varchar
) RETURNS uuid AS $$
DECLARE
    id_for_hash     uuid;
BEGIN
    INSERT INTO storage.object (
        id,
        hash,
        len,
        mime_type
    ) VALUES (
        object_id,
        object_hash,
        object_len,
        object_mime_type
    ) ON CONFLICT DO NOTHING;

    SELECT id INTO id_for_hash
    FROM storage.object
    WHERE hash = object_hash;

    RETURN id_for_hash;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION storage.delete_bucket(
    bucket_id       uuid
) RETURNS void AS $$
BEGIN
    DELETE FROM bucket
    WHERE id = bucket_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION storage.delete_orphan_objects()
RETURNS SETOF object AS $$
BEGIN
    RETURN QUERY
    DELETE FROM object USING object_reference
    WHERE id = object_id AND reference_count = 0
    RETURNING id, hash, len, mime_type, date_added;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION storage.remove_object(
    bkt_id          uuid,
    obj_id          uuid
) RETURNS SETOF object AS $$
BEGIN
    RETURN QUERY
    WITH deleted AS (
        DELETE FROM bucket_object
        WHERE bucket_id = bkt_id AND object_id = obj_id
        RETURNING object_id, date_added
    )
    SELECT
        object_id,
        hash,
        len,
        mime_type,
        deleted.date_added
    FROM deleted
        JOIN object ON object_id = object.id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION storage.rename_bucket(
    bucket_id       uuid,
    bucket_name     varchar(128)
) RETURNS void AS $$
BEGIN
    UPDATE bucket
    SET name = bucket_name
    WHERE id = bucket_id;
END;
$$ LANGUAGE plpgsql;
