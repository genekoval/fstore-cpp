#include "endpoints.h"

#include <fstore/error.h>

namespace fstore::server {
    auto $message(std::string&& msg) -> std::string {
        INFO() << "Message size: " << msg.size();
        INFO() << "Message: " << msg;

        if (msg == "error") throw fstore_error(
            "message not allowed"
        );

        auto os = std::ostringstream();
        os << "You gave me: `" << msg << "`.\nThank you!";
        return os.str();
    }

    auto message(protocol& proto) -> void {
        proto.reply($message);
    }
}
