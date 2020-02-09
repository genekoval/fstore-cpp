#pragma once

#include <fstore/core.h>

#include <filesystem> // std::filesystem::path
#include <memory> // std::unique_ptr
#include <optional> // std::optional
#include <string_view> // std::string_view

namespace fstore::service {
    /**
     * A representation of data. An object includes the data itself, a
     * variable amount of metadata, and a unique identifier.
     */
    struct object {
        /**
         * Returns this object's unique identifier.
         *
         * returns: This object's id.
         */
        virtual std::string_view id() const = 0;

        /**
         * Returns a value obtained by hashing the contents of this object.
         * Each object has a unique hash value.
         *
         * returns: The hash of this object's contents.
         */
        virtual std::string_view hash() const = 0;

        /**
         * Returns the amount of data this object represents. The size is
         * given in bytes.
         *
         * returns: The size of this object's contents.
         */
        virtual uintmax_t size() const = 0;
    };

    /**
     * A namespace under which objects are stored. A bucket contains zero or
     * more objects. No two buckets may have the same name. Therefore, buckets
     * are identified solely by their names.
     */
    struct bucket {
        /**
         * Adds the specified file to this bucket as an object. The specified
         * file is not modified by this operation.
         *
         * params:
         *      - name: file
         *        desc: The file to add to this bucket.
         * returns: An object with the same contents as the specified file.
         */
        virtual std::unique_ptr<object> add_object(
            const std::filesystem::path& file
        ) = 0;

        /**
         * Deletes this bucket.
         */
        virtual void destroy() = 0;

        /**
         * Returns the name of this bucket.
         *
         * returns: The name of this bucket.
         */
        virtual std::string_view name() const = 0;

        /**
         * Sets the name of this bucket to the specified name.
         *
         * params:
         *      - name: name
         *        desc: The new name for this bucket.
         */
         virtual void name(std::string_view new_name) = 0;

         virtual std::unique_ptr<object> remove_object(
            std::string_view object_id
        ) = 0;
    };

    struct bucket_provider {
        /**
         * Creates a new bucket with the specified name. If creation
         * is successful, an optional containing the new bucket is returned.
         * If the bucket creation fails, an empty optional is returned.
         *
         * Bucket creation fails if a bucket with the specified name already
         * exists.
         *
         * params:
         *      - name: name
         *        desc: The name of the new bucket.
         * returns: The newly created bucket.
         */
        static std::unique_ptr<bucket> create(std::string_view name);

        /**
         * Returns an optional containing an existing bucket with the
         * specified name. If the bucket does not exist, an empty optional is
         * returned.
         *
         * params:
         *      - name: name
         *        desc: The name of the bucket to retrieve.
         * returns: An existing bucket with the specified name.
         */
        static std::optional<std::unique_ptr<bucket>> fetch(
            std::string_view name
        );
    };
}
