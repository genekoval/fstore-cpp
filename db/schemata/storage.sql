DROP SCHEMA IF EXISTS storage CASCADE;

CREATE SCHEMA storage

CREATE TABLE bucket (
    id              uuid PRIMARY KEY,
    name            varchar(128) UNIQUE NOT NULL,
    date_created    timestamptz DEFAULT NOW()
)

CREATE TABLE object (
    -- SHA256 checksum of the file contents.
    hash            char(65) PRIMARY KEY,

    -- Length of the file contents in bytes.
    len             bigint NOT NULL,

    -- The time this object was first added to the database.
    date_added      timestamptz DEFAULT NOW()
)

CREATE TABLE bucket_object (
    bucket_id       uuid REFERENCES bucket(id) ON DELETE CASCADE,
    object_id       char(65) REFERENCES object(hash) ON DELETE CASCADE,
    date_added      timestamptz DEFAULT NOW(),

    PRIMARY KEY (bucket_id, object_id)
)

;

CREATE FUNCTION storage.add_object(
    bucket_name     varchar(128),
    obj_hash        char(65)
) RETURNS void AS $$
BEGIN
    INSERT INTO storage.bucket_object (bucket_id, object_id) VALUES (
        (
            SELECT id
            FROM storage.bucket
            WHERE name = bucket_name
        ),
        obj_hash
    );
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION storage.create_bucket(
    bucket_id       uuid,
    bucket_name     varchar(128)
) RETURNS void AS $$
BEGIN
    INSERT INTO storage.bucket (id, name) VALUES (
        bucket_id,
        bucket_name
    );
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION storage.create_object(
    obj_hash        char(65),
    obj_len         bigint
) RETURNS char(65) AS $$
BEGIN
    INSERT INTO storage.object (hash, len) VALUES (
        obj_hash,
        obj_len
    );

    RETURN obj_hash;
END;
$$ LANGUAGE plpgsql;
