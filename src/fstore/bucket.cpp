#include <commline/commands.h>
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <uuid/uuid.h>

using std::string;

string generate_uuid() {
    uuid_t uuid;
    uuid_generate(uuid);

    char uuid_str[37];
    uuid_unparse(uuid, uuid_str);

    return uuid_str;
}

void commline::commands::bucket(const commline::cli& cli) {
    if (cli.args().empty())
        throw commline::cli_error("No bucket name given.");

    const auto& name = cli.args().front();
    std::cout << "Creating bucket: " << name << std::endl;

    pqxx::connection connection("postgresql://fstore@localhost/fstore");
    pqxx::work transaction(connection);

    transaction.exec(
        "SELECT storage.create_bucket("
            + transaction.quote(generate_uuid()) + ","
            + transaction.quote(cli.args().front()) +
        ")"
    );

    transaction.commit();
}
