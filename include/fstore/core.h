#pragma once

#include <fstore/types.h> // fstore::uuid

#include <filesystem> // std::filesystem::path
#include <memory> // std::unique_ptr
#include <stdexcept> // std::runtime_error
#include <string_view> // std::string_view

namespace fstore::core {
    struct fstore_error : std::runtime_error {
        fstore_error(const std::string& what_arg);
    };

    struct object {
        /**
         * Returns this object's id.
         *
         * returns: This object's id.
         */
        virtual uuid id() const = 0;
    };

    struct bucket {
        /**
         * Creates a new bucket with the specified name.
         *
         * param: name The name of the new bucket.
         * returns: The bucket that was created.
         * exception: fstore::core::fstore_error
         *      when: A bucket with the given name already exists.
         */
        static std::unique_ptr<bucket> create(std::string_view name);

        /**
         * Creates a new object from the file pointed to by the specified
         * path and adds it to this bucket.
         *
         * param file The file from which to create the object.
         */
        virtual void add(const std::filesystem::path& path) = 0;

        /**
         * Returns the name of this bucket.
         *
         * returns: The name of this bucket.
         */
        virtual std::string_view name() const = 0;

        /**
         * Sets the name of this bucket to the specified name.
         *
         * param: name The new name for this bucket.
         */
        // virtual void name(std::string_view new_name) = 0;

        /**
         * Deletes this bucket and all of its objects.
         */
        virtual void remove() = 0;
    };
}
