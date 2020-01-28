#pragma once

#include <fstore/core.h>

#include <filesystem> // std::filesystem::path
#include <memory> // std::unique_ptr
#include <optional> // std::optional
#include <string_view> // std::string_view

namespace fstore::service {
    struct object {
        /**
         * Returns this object's id.
         *
         * returns: This object's id.
         */
        virtual fstore::core::uuid id() const = 0;

        /**
         * Returns a value obtained by hashing the contents of this object.
         *
         * returns: The hash of this object's contents.
         */
        virtual std::string_view hash() const = 0;

        /**
         * Returns the size (in bytes) of this object's contents.
         *
         * returns: The size of this object's contents.
         */
        virtual uintmax_t size() const = 0;
    };

    struct bucket {
        /**
         * Creates a new object from the file pointed to by the specified
         * path and adds it to this bucket as an object.
         *
         * params:
         *      - name: file
         *        desc: The file from which to create the object.
         */
        virtual void add_object(const std::filesystem::path& path) = 0;

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

        /**
         * Deletes this bucket and all of its objects.
         */
        virtual void destroy() = 0;
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
        static std::optional<std::unique_ptr<bucket>> create(
            std::string_view name
        );

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
