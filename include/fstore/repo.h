#pragma once

#include <fstore/service.h>

namespace fstore::repo {
    namespace db {
        class has_uuid {
            core::uuid m_id;
        protected:
            has_uuid() = default;
            has_uuid(std::string_view uuid);
            has_uuid(const core::uuid& uuid);

            void id(std::string_view new_id);
            void nullify();
        public:
            std::string_view id() const;
            bool is_valid() const;
        };

        class bucket_entity : public has_uuid {
            std::string m_name;
        public:
            bucket_entity(const core::uuid& uuid, std::string_view name);
            bucket_entity(std::string_view name);

            void add_object(const service::object& obj);
            void destroy();
            std::string_view name() const;
            void name(std::string_view name);
        };

        class object_entity : public has_uuid {
            std::string m_hash;
            uintmax_t m_size;
        public:
            object_entity(
                const fstore::core::uuid& id,
                std::string_view hash,
                uintmax_t size
            );

            std::string_view hash() const;
            uintmax_t size() const;
        };
    }
}
