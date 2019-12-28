-- postgresql 12.1

DROP SCHEMA IF EXISTS storage CASCADE;

CREATE SCHEMA storage

CREATE TABLE bucket (
    id              uuid PRIMARY KEY,
    name            varchar(128) NOT NULL,
    date_created    date NOT NULL
)

CREATE TABLE object (
    -- SHA256 checksum of the file contents.
    hash            char(64) PRIMARY KEY,

    -- Length of the file contents in bytes.
    len             bigint NOT NULL,

    -- The time this object was first added to the database.
    date_added      date NOT NULL
)

CREATE TABLE bucket_object (
    bucket_id       uuid REFERENCES bucket(id) ON DELETE CASCADE,
    object_id       char(64) REFERENCES object(hash) ON DELETE CASCADE,
    date_added      date NOT NULL,

    PRIMARY KEY (bucket_id, object_id)
)

; -- END SCHEMA storage
