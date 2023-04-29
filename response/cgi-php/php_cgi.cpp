#include "../response.hpp"


int responseClient::read_data_from_cgi()
{
    client->flagResponse->file_RW.close();
    client->flagResponse->file_RW.open(client->flagResponse->tmp_file.str());
    if (!client->flagResponse->file_RW)
        std::cout << "Error" << std::endl;
    client->flagResponse->file_RW.seekg(0, std::ios::end);
    client->flagResponse->content_length = client->flagResponse->file_RW.tellg();
    client->flagResponse->file_RW.seekg(0, std::ios::beg);
    found = 0;
    std::cout << "content " << client->flagResponse->content_length << std::endl;
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


std::string& responseClient::pars_cgi_uri()
{
    std::cout << "pars CGI arg " << std::endl;

    found = uri.find("?");
    aEnv.push_back("QUERY_STRING=" + uri.substr(found + 1));
    uri = uri.substr(0, uri.find("?"));
    return uri;
}

int    responseClient::handle_cgi() 
{ 
    char **sysEnv = NULL;
    
    std::cout << "run cgi scripts " << std::endl;
    // Create file for communication between parent and child processes
    int t_fd[2];
    client->flagResponse->tmp_file << "../upload/" << rawtime <<  "_cgi_" << client->socket;

    // ----- ----- read body request and using in for cgi ----- ----- //
    if (client->request_data_struct->method == "POST")
    {
        unlink((client->flagResponse->tmp_file.str() + "post").c_str());
        t_fd[0] = open((client->flagResponse->tmp_file.str() + "post").c_str(), O_RDWR | O_CREAT, 0664 );
        client->flagResponse->file_RW.close();
        client->flagResponse->file_RW.open((client->flagResponse->tmp_file.str() + "post"));
        client->flagResponse->file_RW << client->request_data_struct->body;
        close(t_fd[0]);
        t_fd[0] = open((client->flagResponse->tmp_file.str() + "post").c_str(), O_RDONLY);
        client->flagResponse->file_RW.close();
    }
    

    t_fd[1] = open(client->flagResponse->tmp_file.str().c_str(), O_RDWR | O_CREAT, 0664 );
    if (uri.find("?") != std::string::npos)
        pars_cgi_uri();
    
    std::cout << "cgi path  " << block_server[nBlock].list_of_location[nbrLocation].cgi_path << std::endl;
    std::cout << "uri " << uri << std::endl;

    char *argv[3];
    argv[0] = (char*)block_server[nBlock].list_of_location[nbrLocation].cgi_path.c_str();// check cgi path
    argv[1] = (char *) uri.c_str();
    argv[2] = NULL;

    //////////////////////////////////////////////////
    tmp_string = "SCRIPT_FILENAME=";
    tmp_string += argv[1];
    aEnv.push_back("REDIRECT_STATUS=200");
    aEnv.push_back(tmp_string);
    aEnv.push_back("REQUEST_METHOD=" + client->request_data_struct->method);
    aEnv.push_back("CONTENT_TYPE=" +  client->request_data_struct->content_Type);
    aEnv.push_back("CONTENT_LENGTH="+ client->request_data_struct->content_Length);
    aEnv.push_back("HTTP_COOKIE="+ client->request_data_struct->cookie);

    sysEnv = new char*[aEnv.size() + 1];
    for (size_t i=0; i<aEnv.size(); i++)
    {
        sysEnv[i] = (char *)aEnv[i].c_str();
        std::cout << "sysEnv " << sysEnv[i] << std::endl;
    }
        //////////////////////////////////////////////////
    sysEnv[aEnv.size()] = NULL;

/////----- execute cgi file  
    pid_t pid = fork();
    if (pid == -1) {
            unlink(client->flagResponse->tmp_file.str().c_str());
        // field fork
        close(t_fd[0]);
        close(t_fd[1]);
        return 1;
    } 
    else if (pid == 0) {
        // Child process
        dup2(t_fd[0], 0);
        dup2(t_fd[1], 1);
        close(t_fd[0]);
        close(t_fd[1]);
        execve(argv[0], argv, sysEnv);

        write(1, "failed1\n", 9);
        exit(1);
    }
    else {
        // Parent process
        // Read output from child process

        ///get conetent lenght file
        int status;
        waitpid(pid, &status, 0);

        read_data_from_cgi();
        delete(sysEnv);

        buff.clear();
        buff  << "HTTP/1.1 " << statusCodes["200"] \
                << "\r\nContent-Length: " << client->flagResponse->content_length \
                << "\r\n" << tmp_string ;
                std::cout << "xheck header " << tmp_string << std::endl;
        write(client->socket, buff.str().c_str(), buff.str().length());
        if (!client->flagResponse->content_length)
        {
            client->flagResponse->isReading = false;
            client->flagResponse->file_RW.close();
            unlink(client->flagResponse->tmp_file.str().c_str());
            unlink((client->flagResponse->tmp_file.str() + "post").c_str());
            drop_client(client);
        }
        std::cout << "cgi send" << std::endl;
        // Wait for child process to exit
    }
    return 0;
}