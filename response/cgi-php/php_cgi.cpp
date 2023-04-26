#include "../response.hpp"


int    responseClient::handle_cgi() 
{
    std::cout << "run cgi scripts " << std::endl;
    // Create file for communication between parent and child processes
    int t_fd[2];
    char tmp[256];
    std::string tmp_file("./cgi.txt" );
    std::string tempDer(getcwd(tmp, 256));

    tempDer += "/cgi-php/php-cgi";

    t_fd[0] = open(tmp_file.c_str(), O_RDWR | O_CREAT);
    t_fd[1] = open(tmp_file.c_str(), O_RDONLY);
    // char *argv[] = {(char*)"php-cgi", (char*)"index.php", NULL};
    std::cout << "temp = " << getcwd(tmp, 256) << std::endl;
    char *envp[] = {(char*)"ENV_VAR1=value1", (char*)"ENV_VAR2=value2", NULL};
    char *argv[] = {(char*)"/bin/echo", (char*)"/Users/hjrifi/Desktop/serr/response/cgi-php/index.php", NULL};

    pid_t pid = fork();
    if (pid == -1) {
        // field fork
        close(t_fd[0]);
        close(t_fd[1]);
        return 1;
    } 
    else if (pid == 0) {
        // Child process
        dup2(t_fd[0], 1);
        close(t_fd[0]);
        execve("/Users/hjrifi/Desktop/serr/cgi-php/php-cgi", argv, envp);
        exit(1);
    }
    else {
        // Parent process
        close(t_fd[1]);
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
        client->flagResponse->isReading = false;
        while ((n = read(t_fd[1], buf, sizeof(buf))) > 0) {
            write(1, buf, n);
        }
        client->flagResponse->isReading = false;
        drop_client(client);
        std::cout << "cgi send" << std::endl;
        // close(t_fd[0]);
        // Wait for child process to exit
        int status;
        waitpid(pid, &status, 0);
    }
    return 0;
}