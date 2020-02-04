#pragma once

#include <fstore/service.h>

namespace fstore {
    std::unique_ptr<service::bucket> fetch_bucket(
        const std::string& bucket_name
    );
}
