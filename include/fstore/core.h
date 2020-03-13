#pragma once

#include <cstdint> // uintmax_t
#include <filesystem> // std::filesystem::path
#include <memory> // std::unique_ptr
#include <optional> // std::optional
#include <string_view> // std::string_view
#include <vector> // std::vector

namespace fstore::core {
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
        virtual auto id() const -> std::string_view = 0;

        /**
         * Returns a value obtained by hashing the contents of this object.
         * Each object has a unique hash value.
         *
         * returns: The hash of this object's contents.
         */
        virtual auto hash() const -> std::string_view = 0;

        /**
         * Returns the mime type of this object's contents.
         *
         * returns: The mime type of this object's contents.
         */
        virtual auto mime_type() const -> std::string_view = 0;

        /**
         * Returns the amount of data this object represents. The size is
         * given in bytes.
         *
         * returns: The size of this object's contents.
         */
        virtual auto size() const -> uintmax_t = 0;
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
        virtual auto add_object(const std::filesystem::path& file) ->
            std::unique_ptr<object> = 0;

        /**
         * Deletes this bucket.
         */
        virtual auto drop() -> void = 0;

        /**
         * Returns the name of this bucket.
         *
         * returns: The name of this bucket.
         */
        virtual auto name() const -> std::string_view = 0;

        /**
         * Sets the name of this bucket to the specified name.
         *
         * params:
         *      - name: name
         *        desc: The new name for this bucket.
         */
         virtual auto name(std::string_view new_name) -> void = 0;

         virtual auto object_count() const -> int = 0;

         virtual auto remove_object(std::string_view object_id) ->
            std::unique_ptr<object> = 0;

        virtual auto space_used() const -> uintmax_t = 0;
    };

    struct store_totals {
        virtual auto bucket_count() const -> int = 0;
        virtual auto object_count() const -> int = 0;
        virtual auto space_used() const -> uintmax_t = 0;
    };

    struct object_store {
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
        virtual auto create_bucket(std::string_view name) const ->
            std::unique_ptr<bucket> = 0;

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
        virtual auto fetch_bucket(std::string_view name) const ->
            std::optional<std::unique_ptr<bucket>> = 0;

        virtual auto fetch_buckets() const ->
            std::vector<std::unique_ptr<bucket>> = 0;

        virtual auto fetch_buckets(
            const std::vector<std::string>& names
        ) const -> std::vector<std::unique_ptr<bucket>> = 0;

        virtual auto get_store_totals() const ->
            std::unique_ptr<core::store_totals> = 0;

        /**
         * Removes all objects that are not referenced by a bucket.
         */
        virtual auto prune() const -> std::vector<std::unique_ptr<object>> = 0;
    };
}
