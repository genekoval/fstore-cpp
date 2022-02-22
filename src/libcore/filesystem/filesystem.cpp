#include "crypto/crypto.h"

#include <fstore/core/filesystem.h>

#include <ext/unix.h>
#include <fcntl.h>
#include <fmt/format.h>
#include <fstream>
#include <magix.h>
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
        std::filesystem::create_directories(objects);
        std::filesystem::create_directories(parts);
    }

    auto filesystem::copy(
        const std::filesystem::path& source,
        std::string_view id
    ) const -> void {
        const auto object = path_to(id);

        std::filesystem::copy_file(
            source,
            object,
            std::filesystem::copy_options::skip_existing
        );

        std::filesystem::permissions(object, object_permmissions);
    }

    auto filesystem::get_part(std::string_view id) const -> std::ofstream {
        auto path = parts/id;
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

    auto filesystem::make_object(std::string_view part_id) -> void {
        const auto part = part_path(part_id);
        const auto object = path_to(part_id);

        std::filesystem::rename(part, object);
        std::filesystem::permissions(object, object_permmissions);
    }

    auto filesystem::mime_type(
        const std::filesystem::path& path
    ) const -> std::string {
        return magic_mime_type().file(path);
    }

    auto filesystem::open(std::string_view id) const -> netcore::fd {
        auto path = path_to(id);
        return ::open(path.c_str(), O_RDONLY);
    }

    auto filesystem::part_path(
        std::string_view id
    ) const -> std::filesystem::path {
        return parts/id;
    }

    auto filesystem::path_to(
        std::string_view id
    ) const -> std::filesystem::path {
        return objects/id;
    }

    auto filesystem::remove(std::string_view id) const -> void {
        std::filesystem::remove(path_to(id));
    }

    auto filesystem::remove_part(std::string_view id) const -> void {
        std::filesystem::remove(parts/id);
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

        if (timber::reporting_level >= timber::level::debug) {
            auto os = std::ostringstream();

            os << "EXEC " << program;
            for (const auto& arg : args) os << " " << arg;

            DEBUG() << os.str();
        }

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
