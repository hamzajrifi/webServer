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
    size_t start_cgi = get_current_time('s');
    client->flagResponse->pid = fork();
    if (client->flagResponse->pid == -1) {
        // field fork
        unlink(client->flagResponse->tmp_file.str().c_str());
        close(cgi_fd[0]);
        close(cgi_fd[1]);
        return 1;
    } 
    else if (client->flagResponse->pid == 0) {
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
        size_t nbr_time_out = atoi(block_server[nBlock].fastcgi_read_timeout.c_str());
        while (waitpid(client->flagResponse->pid, &status, WNOHANG) == 0)
        {
            if (get_current_time('s') > start_cgi + (block_server[nBlock].fastcgi_read_timeout.empty() ? 10 \
                : (nbr_time_out <= 0 ? 10 : nbr_time_out)))
                kill(client->flagResponse->pid, SIGSEGV);
            usleep(10000);
        
        }
        delete(sysEnv);
        read_data_from_cgi();
        buff.clear();
        buff  << "HTTP/1.1 " << (tmp_string.find("Location: ") > tmp_string.length() ? statusCodes["200"] : statusCodes["301"]) \
                << (tmp_string.find("Content-Type: ") > tmp_string.length() ? "\r\nContent-Type: text/html\r\n" : "\r\n")\
                << "Content-Length: " << client->flagResponse->content_length \
                << "\r\n" << tmp_string;
        if (sendHeader(client->socket, buff.str().c_str(), buff.str().length()))
            return -1;
        if (!client->flagResponse->content_length)
        {
            client->flagResponse->isReading = false;
            client->flagResponse->file_RW.close();
            unlink(client->flagResponse->tmp_file.str().c_str());
            unlink((client->flagResponse->tmp_file.str() + "post").c_str());
            drop_client(client);
        }
    }
    return 0;
}

std::string& responseClient::pars_cgi_uri()
{
    found = uri.find("?");
    aEnv.push_back("QUERY_STRING=" + uri.substr(found + 1));
    uri = uri.substr(0, uri.find("?"));
    return uri;
}

int    responseClient::handle_cgi() 
{
    // Create file for communication between parent and cgi processes
    client->flagResponse->tmp_file << "/tmp/" << get_current_time('m') <<  "_cgi_" << client->socket;

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

    if ((found = client->request_data_struct->cookie.find("session") == std::string::npos
        && (client->request_data_struct->cookie[found] == '?' 
            || client->request_data_struct->cookie[found] == '&')))
        session.push_back(nbr_session_client++);
    
    sysEnv = new char*[aEnv.size() + 1];
    for (size_t i=0; i<aEnv.size(); i++)
       sysEnv[i] = (char *)aEnv[i].c_str();
    sysEnv[aEnv.size()] = NULL;
/////----- execute cgi file  
    execute_cgi_file();

    return 0;
}