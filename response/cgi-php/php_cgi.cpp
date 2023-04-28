#include "../response.hpp"


int responseClient::read_data_from_cgi()
{
    client->flagResponse->file_RW.close();
    client->flagResponse->file_RW.open(client->flagResponse->tmp_file.str());
    client->flagResponse->file_RW.seekg(0, std::ios::end);
    client->flagResponse->content_length = client->flagResponse->file_RW.tellg();
    client->flagResponse->file_RW.seekg(0, std::ios::beg);
    size_t found = 0;
    while (1337)
    {
        client->flagResponse->file_RW.read(res_body, BSIZE);
        if (client->flagResponse->file_RW.gcount() <= 0)
            break;
        tmp_string += res_body;
        found = tmp_string.find("\r\n\r\n");
        if (found != std::string::npos)
        {
            found += 4;
            tmp_string = tmp_string.substr(0, found);
            break;
        }
    }
    std::cout << "found " << found << std::endl;

    client->flagResponse->file_RW.close();
    client->flagResponse->file_RW.open(client->flagResponse->tmp_file.str());
    client->flagResponse->file_RW.seekg(found, std::ios::beg);
    client->flagResponse->content_length -= found;

    return 0;
}


int responseClient::pars_cgi_uri()
{
    uri = uri.substr(0, uri.find("?"));
    std::cout << "inside " << std::endl;
    return 0;
}

int    responseClient::handle_cgi() 
{
    std::cout << "run cgi scripts " << std::endl;
    // Create file for communication between parent and child processes
    int t_fd;
    client->flagResponse->tmp_file << "/tmp/" << rawtime <<  "_cgi_" << client->socket;

    t_fd = open(client->flagResponse->tmp_file.str().c_str(), O_RDWR | O_CREAT, 0664 );
    if (uri.find("?") != std::string::npos)
        pars_cgi_uri();
    std::cout << "uri " << uri << std::endl;
    char *argv[3];
    argv[0] = (char*)"php-cgi";
    argv[1] = (char *) uri.c_str();
    argv[2] = NULL;

    char *envp[6];
    std::string tmpEnv_Uri = "SCRIPT_FILENAME=";
    tmpEnv_Uri.append(argv[1]);
    envp[0] = (char *)"REQUEST_METHOD=GET";
    envp[1] = (char *)tmpEnv_Uri.c_str();
    envp[2] = (char *)"REDIRECT_STATUS=200";
    envp[3] = (char *)"name=lcom";
    envp[4] = (char *)"version=4.8.1";
    envp[5] = NULL;


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

        read_data_from_cgi();

        buff.clear();
        buff << "HTTP/1.1 " << "\r\nContent-Length: " << client->flagResponse->content_length \
                << "\r\n" << tmp_string ;
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