#include "crypto/crypto.hpp"

#include <internal/core/fs/filesystem.hpp>

#include <ext/unix.h>
#include <fcntl.h>
#include <fmt/std.h>
#include <fstream>
#include <magix/magix>
#include <timber/timber>
#include <vector>

namespace {
    constexpr auto object_permmissions =
        std::filesystem::perms::owner_read |
        std::filesystem::perms::owner_write |
        std::filesystem::perms::group_read;

    constexpr auto sync_options = std::array {
        "--archive",
        "--delete"
    };

    constexpr auto object_dir = "objects";
    constexpr auto parts_dir = "parts";

    auto magic_mime_type() -> magix::magic {
        return magix::magic(MAGIC_MIME_TYPE);
    }
}

namespace fstore::core::fs {
    filesystem::filesystem(const std::filesystem::path& home) :
        objects(home/object_dir),
        parts(home/parts_dir)
    {
        create_directories(objects);
        create_directories(parts);
    }

    auto filesystem::copy(
        const std::filesystem::path& source,
        std::string_view id
    ) const -> void {
        make_object(id, [&source](const auto& dest) {
            if (std::filesystem::copy_file(
                source,
                dest,
                std::filesystem::copy_options::skip_existing
            )) {
                TIMBER_DEBUG("Copied file: {} -> {}", source, dest);
            }
        });
    }

    auto filesystem::create_directories(
        const std::filesystem::path& path
    ) const -> void {
        if (std::filesystem::create_directories(path)) {
            TIMBER_DEBUG("Created directories: {}", path);
        }
    }

    auto filesystem::get_part(std::string_view id) const -> std::ofstream {
        auto path = part_path(id);
        return std::ofstream(
            path,
            std::ios::out | std::ios::app | std::ios::binary
        );
    }

    auto filesystem::hash(
        std::span<const std::byte> buffer
    ) const -> std::string {
        return crypto::sha256sum(buffer);
    }

    auto filesystem::hash(
        const std::filesystem::path& path
    ) const -> std::string {
        return crypto::sha256sum(path);
    }

    auto filesystem::make_object(std::string_view part_id) const -> void {
        const auto part = part_path(part_id);

        make_object(part_id, [&part](const auto& dest) {
            std::filesystem::rename(part, dest);
            TIMBER_DEBUG("Renamed file: {} -> {}", part, dest);
        });
    }

    auto filesystem::make_object(
        std::string_view object_id,
        std::function<void(const std::filesystem::path&)>&& action
    ) const -> void {
        const auto path = object_path(object_id);
        create_directories(path.parent_path());

        action(path);

        std::filesystem::permissions(path, object_permmissions);
    }

    auto filesystem::mime_type(
        const std::filesystem::path& path
    ) const -> mime {
        const auto type = magic_mime_type().file(path);
        const auto index = type.find("/");

        return {
            .type = type.substr(0, index),
            .subtype = type.substr(index + 1)
        };
    }

    auto filesystem::object_path(
        std::string_view id
    ) const -> std::filesystem::path {
        return objects / id.substr(0, 2) / id.substr(2, 2) / id;
    }

    auto filesystem::open(std::string_view id) const -> netcore::fd {
        const auto path = object_path(id);
        return netcore::open(path, O_RDONLY);
    }

    auto filesystem::part_path(
        std::string_view id
    ) const -> std::filesystem::path {
        return parts/id;
    }

    auto filesystem::remove(std::string_view id) const -> void {
        std::filesystem::remove(object_path(id));
    }

    auto filesystem::remove_part(std::string_view id) const -> void {
        const auto part = part_path(id);

        if (std::filesystem::remove(part)) {
            TIMBER_DEBUG("Removed part: {}", part);
        }
        else {
            TIMBER_WARNING("Tried to remove nonexistent file: {}", part);
        }
    }

    auto filesystem::size(
        const std::filesystem::path& path
    ) const -> uintmax_t {
        return std::filesystem::file_size(path);
    }

    auto filesystem::sync(
        std::string_view program,
        std::span<const std::string_view> options,
        std::string_view dest
    ) const -> void {
        auto args = std::vector<std::string_view>();

        std::copy(
            sync_options.begin(),
            sync_options.end(),
            std::back_inserter(args)
        );
        std::copy(options.begin(), options.end(), std::back_inserter(args));
        const auto src = objects.string();
        args.push_back(src);
        args.push_back(dest);

        TIMBER_DEBUG("EXEC {} {}", program, fmt::join(args, " "));

        const auto exit = ext::wait_exec(program, args);

        if (exit.code != CLD_EXITED) throw std::runtime_error(fmt::format(
            "{} did not exit properly",
            program
        ));

        if (exit.status != 0) throw std::runtime_error(fmt::format(
            "{} exited with code {}",
            program,
            exit.status
        ));
    }
}
