#include <fstore/test.h>
#include <fstore/repo/filesystem.h>
#include <fstore/server/server.h>

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

namespace fstore::test {
    constexpr auto ignored = -1;

    auto start_server(const std::filesystem::path& unix_socket) -> pid_t {
        if (std::filesystem::exists(unix_socket)) return ignored;

        int pipefd[2];
        pipe(pipefd);

        auto ready = false;

        auto pid = fork();

        if (pid > 0) {
            close(pipefd[1]);
            read(pipefd[0], &ready, sizeof(decltype(ready)));

            if (ready) return pid;

            throw std::runtime_error("failed to start server");
        }

        close(pipefd[0]);

        auto objects = temp_directory_path() / "objects";
        auto store = service::object_store(
            db(),
            repo::fs::get(objects)
        );

        server::listen(store, unix_socket.string(), [&]() {
            INFO() << "Listening for connections on: " << unix_socket;
            ready = true;
            write(pipefd[1], &ready, sizeof(decltype(ready)));
        });

        std::filesystem::remove_all(objects);

        std::exit(EXIT_SUCCESS);
    }

    auto stop_server(pid_t pid) -> void {
        if (pid == ignored) return;

        int status;

        kill(pid, SIGINT);
        wait(&status);
    }
}
