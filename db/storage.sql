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
