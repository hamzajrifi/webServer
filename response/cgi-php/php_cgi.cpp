#include "../response.hpp"


int    responseClient::handle_cgi() 
{
    std::cout << "run cgi scripts " << std::endl;
    // Create pipe for communication between parent and child processes
    int pipe_fds[2];
    pipe(pipe_fds);
    // Fork a child process to execute the CGI script
    // char php_file[] = "../../pages.php";
    // char php_executable[2][20] = {"/usr/bin/php" ,"./../pages.php"};
    char *argv[] = {(char*)"php-cgi", (char*)"index.php", NULL};

    char *envp[] = {(char*)"ENV_VAR1=value1", (char*)"ENV_VAR2=value2", NULL};

    pid_t pid = fork();
    if (pid == -1) {
        // Error occurred
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return 1;
    } 
    else if (pid == 0) {
        // Child process
        close(pipe_fds[0]);
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[1]);
        execve("/usr/bin/php", argv, envp);
        exit(1);
    } 
    else {
        // Parent process
        close(pipe_fds[1]);
        // Read output from child process
        char buf[1024];
        ssize_t n;
        buff.clear();
        buff << "HTTP/1.1 " << statusCodes["200"] \
                << "\r\nContent-Type: "  << "text/plain"\
                << "\r\nContent-Length: " << "4" \
                << "\r\nConnection: close\r\n"\
                << "\r\r1111";
        write(client->socket, buff.str().c_str(), buff.str().length());
        return 0;
        while ((n = read(pipe_fds[0], buf, sizeof(buf))) > 0) {
            write(client->socket, buf, n);
        }
        client->flagResponse->isReading = false;
        drop_client(client);
        std::cout << "cgi send" << std::endl;
        close(pipe_fds[0]);
        // Wait for child process to exit
        int status;
        waitpid(pid, &status, 0);
    }
    return 0;
}