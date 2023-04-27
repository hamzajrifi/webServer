#include "../response.hpp"


int    responseClient::handle_cgi() 
{
    std::cout << "run cgi scripts " << std::endl;
    // Create file for communication between parent and child processes
    int t_fd;
    client->flagResponse->tmp_file << "/tmp/" << rawtime <<  "_cgi_" << client->socket;

    t_fd = open(client->flagResponse->tmp_file.str().c_str(), O_RDWR | O_CREAT, 0664 );
    std::cout << "uri " << uri << std::endl;

    char *argv[3];
    argv[0] = (char*)"php-cgi";
    argv[1] = (char *) uri.c_str();
    argv[2] = NULL;

    char *envp[4];
    std::string tmpEnv_Uri = "SCRIPT_FILENAME=";
    tmpEnv_Uri.append(argv[1]);
    envp[0] = (char *)"REQUEST_METHOD=GET";
    envp[1] = (char *)tmpEnv_Uri.c_str();
    envp[2] = (char *)"REDIRECT_STATUS=200";
    envp[3] = NULL;


    pid_t pid = fork();
    if (pid == -1) {
            unlink(client->flagResponse->tmp_file.str().c_str());
        // field fork
        close(t_fd);
        return 1;
    } 
    else if (pid == 0) {
        // Child process
        dup2(t_fd, 1);
        close(t_fd);
        execve("../cgi-php/php-cgi", argv, envp);

        write(1, "failed1\n", 7);
        exit(1);
    }
    else {
        // Parent process
        // Read output from child process

        ///get conetent lenght file
        int status;
        waitpid(pid, &status, 0);
        client->flagResponse->file_RW.close();
        client->flagResponse->file_RW.open(client->flagResponse->tmp_file.str());
		client->flagResponse->file_RW.seekg(0, std::ios::end);
		client->flagResponse->content_length = client->flagResponse->file_RW.tellg();
		client->flagResponse->file_RW.seekg(0,std::ios::beg);

        buff.clear();
        std::cout << "length = " << client->flagResponse->content_length << std::endl;
        buff << "HTTP/1.1 " << statusCodes["200"] \
                << "\r\nContent-Type: "  << "text/html"\
                << "\r\nContent-Length: " << client->flagResponse->content_length \
                << "\r\nConnection: close\r\n"\
                << "\r\n";
        write(client->socket, buff.str().c_str(), buff.str().length());
        if (!client->flagResponse->content_length)
        {
            client->flagResponse->isReading = false;
            client->flagResponse->file_RW.close();
            unlink(client->flagResponse->tmp_file.str().c_str());
            drop_client(client);
        }
        std::cout << "cgi send" << std::endl;
        // Wait for child process to exit
    }
    return 0;
}