#include "../response.hpp"


int responseClient::read_data_from_cgi()
{
    client->flagResponse->file_RW.close();
    client->flagResponse->file_RW.open(client->flagResponse->tmp_file.str());
    found = 0;
    tmp_string.clear();
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
    client->flagResponse->file_RW.close();
    client->flagResponse->file_RW.open(client->flagResponse->tmp_file.str());
    client->flagResponse->file_RW.seekg(0, std::ios::end);
    client->flagResponse->content_length = client->flagResponse->file_RW.tellg();
    client->flagResponse->content_length -= found;
    client->flagResponse->file_RW.seekg(found, std::ios::beg);

    return 0;
}

int responseClient::execute_cgi_file()
{
    pid_t pid = fork();
    if (pid == -1) {
        // field fork
        unlink(client->flagResponse->tmp_file.str().c_str());
        close(cgi_fd[0]);
        close(cgi_fd[1]);
        return 1;
    } 
    else if (pid == 0) {
        // cgi process
        dup2(cgi_fd[0], 0);
        dup2(cgi_fd[1], 1);
        close(cgi_fd[0]);
        close(cgi_fd[1]);
        execve(argv[0], argv, sysEnv);
        exit(1);
    }
    else {
        //------ Read output from child process
        int status;
        // Wait for child process to exit
        // kill(pid, SIGSEGV);
        waitpid(pid, &status, 0);

        delete(sysEnv);
        read_data_from_cgi();

        buff.clear();
        buff  << "HTTP/1.1 " << statusCodes["200"] \
                << (tmp_string.find("Content-Type:") > tmp_string.length() ? "text/html\r\n" : "\r\n")\
                << "Content-Length: " << client->flagResponse->content_length \
                << "\r\n" << tmp_string;
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
    }
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
    std::cout << "run cgi scripts " << std::endl;
    // Create file for communication between parent and cgi processes
    client->flagResponse->tmp_file << "../upload/" << rawtime <<  "_cgi_" << client->socket;

    // ----- ----- read body request and using it for cgi ----- ----- //
    if (client->request_data_struct->method == "POST")
    {
        unlink((client->flagResponse->tmp_file.str() + "post").c_str());
        unlink((client->flagResponse->tmp_file.str()).c_str());
        cgi_fd[0] = open((client->flagResponse->tmp_file.str() + "post").c_str(), O_RDWR | O_CREAT, 0664 );
        client->flagResponse->file_RW.close();
        client->flagResponse->file_RW.open((client->flagResponse->tmp_file.str() + "post"));
        client->flagResponse->file_RW << client->request_data_struct->body;
        close(cgi_fd[0]);
        cgi_fd[0] = open((client->flagResponse->tmp_file.str() + "post").c_str(), O_RDONLY);
        client->flagResponse->file_RW.close();
    }
    cgi_fd[1] = open(client->flagResponse->tmp_file.str().c_str(), O_RDWR | O_CREAT, 0664 );
    if (uri.find("?") != std::string::npos)
        pars_cgi_uri();
    argv[0] = (char*)block_server[nBlock].list_of_location[nbrLocation].cgi_path.c_str();// check cgi path
    argv[1] = (char *) uri.c_str();
    argv[2] = NULL;
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
        sysEnv[i] = (char *)aEnv[i].c_str();
    sysEnv[aEnv.size()] = NULL;
/////----- execute cgi file  
    execute_cgi_file();

    return 0;
}