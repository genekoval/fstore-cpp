#include <fstore/core/filesystem.h>
#include <fstore/test.h>
#include <fstore/repo/filesystem.h>
#include <fstore/server/server.h>

#include <signal.h>
#include <sys/wait.h>
#include <timber/timber>
#include <unistd.h>

namespace fstore::test {
    constexpr auto ignored = -1;

    auto start_server(const std::filesystem::path& unix_socket) -> pid_t {
        if (std::filesystem::exists(unix_socket)) return ignored;

        int pipefd[2];
        pipe(pipefd);

        auto ready = false;

        auto pid = fork();

        if (pid > 0) { // Client
            close(pipefd[1]);
            read(pipefd[0], &ready, sizeof(decltype(ready)));

            if (ready) return pid;
            throw std::runtime_error("failed to start server");
        }
        else { // Server
            close(pipefd[0]);

            auto home = temp_directory();
            auto fs = core::filesystem(home.path);
            auto store = core::object_store(db(), fs);

            const auto info = server::server_info { .version = "0.0.0" };

            server::listen(store, info, { .path = unix_socket }, [&]() {
                INFO() << "Listening for connections on: " << unix_socket;
                ready = true;
                write(pipefd[1], &ready, sizeof(decltype(ready)));
            });
        }

        return pid;
    }

    auto stop_server(pid_t pid) -> void {
        if (pid == ignored) return;

        int status;

        kill(pid, SIGINT);
        wait(&status);
    }
}
