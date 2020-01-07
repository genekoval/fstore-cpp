DROP SCHEMA IF EXISTS storage CASCADE;

CREATE SCHEMA storage

CREATE TABLE bucket (
    id              uuid PRIMARY KEY,
    name            varchar(128) UNIQUE NOT NULL,
    date_created    timestamptz DEFAULT NOW()
)

CREATE TABLE object (
    id              uuid PRIMARY KEY,

    -- SHA256 checksum of the file contents.
    hash            char(65) UNIQUE NOT NULL,

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
        (
            SELECT id
            FROM storage.object
            WHERE hash = obj_hash
        )
    );
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
    obj_hash        char(65),
    obj_len         bigint
) RETURNS char(65) AS $$
BEGIN
    INSERT INTO storage.object (
        id,
        hash,
        len
    ) VALUES (
        obj_id,
        obj_hash,
        obj_len
    );

    RETURN obj_hash;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION storage.delete_bucket(
    bucket_name     varchar(128)
) RETURNS boolean as $$
DECLARE
    rows_affected   integer;
BEGIN
    WITH deleted AS (
        DELETE FROM storage.bucket
        WHERE name = bucket_name
        RETURNING *
    )
    SELECT count(*) INTO rows_affected
    FROM deleted;

    RETURN rows_affected > 0;
END;
$$ LANGUAGE plpgsql;
