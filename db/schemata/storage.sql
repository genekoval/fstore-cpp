DROP SCHEMA IF EXISTS storage CASCADE;

CREATE SCHEMA storage

CREATE TABLE bucket (
    id              uuid PRIMARY KEY,
    name            varchar(128) UNIQUE NOT NULL CHECK (length(trim(name)) <> 0),
    date_created    timestamptz DEFAULT NOW()
)

CREATE TABLE object (
    id              uuid PRIMARY KEY,

    -- SHA256 checksum of the file contents.
    hash            char(64) UNIQUE NOT NULL,

    -- Length of the file contents in bytes.
    len             bigint NOT NULL,

    -- The time this object was first added to the database.
    date_added      timestamptz DEFAULT NOW()
)

CREATE TABLE bucket_object (
    bucket_id       uuid REFERENCES bucket(id) ON DELETE CASCADE,
    object_id       uuid REFERENCES object(id) ON DELETE CASCADE,
    date_added      timestamptz DEFAULT NOW(),

    PRIMARY KEY (bucket_id, object_id)
)

;

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
    obj_id          uuid,
    obj_hash        char(64),
    obj_len         bigint
) RETURNS uuid AS $$
DECLARE
    id_for_hash     uuid;
BEGIN
    INSERT INTO storage.object (
        id,
        hash,
        len
    ) VALUES (
        obj_id,
        obj_hash,
        obj_len
    ) ON CONFLICT DO NOTHING;

    SELECT id INTO id_for_hash
    FROM storage.object
    WHERE hash = obj_hash;

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
