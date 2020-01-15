#include <service.h>

#include <commline/commands.h>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

void commline::commands::add(const commline::cli& cli) {
    if (cli.args().size() < 2)
        throw commline::cli_error("bucket and filename missing");

    service::add_object(
        cli.args()[0],
        cli.args()[1]
    );

    cout << "object added: " << cli.args()[1] << endl;
}
