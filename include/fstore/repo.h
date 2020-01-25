#pragma once

#include <fstore/core.h>
#include <fstore/types.h>

namespace fstore::repo {
    namespace db {
        class has_id {
        protected:
            uuid m_id;

            has_id() = default;
            has_id(const uuid& id);
        public:
            uuid id() const;
        };

        class bucket_entity : public has_id {
            std::string m_name;
        public:
            bucket_entity(const uuid& id, std::string_view name);
            bucket_entity(std::string_view name);

            void add_object(const core::object& obj);
            void destroy();
            std::string_view name();
            void name(std::string_view name);
        };

        class object_entity : public has_id {
            std::string m_hash;
            uintmax_t m_size;
        public:
            object_entity(
                const uuid& id,
                std::string_view hash,
                uintmax_t size
            );

            std::string_view hash() const;
            uintmax_t size() const;
        };
    }
}
