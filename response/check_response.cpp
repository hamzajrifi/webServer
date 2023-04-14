#include "response.hpp"


const char *get_content_type(const char* path) 
{
    const char *last_dot = strrchr(path, '.');

    if (last_dot) {
        if (strcmp(last_dot, ".css") == 0) return "text/css";
        if (strcmp(last_dot, ".csv") == 0) return "text/csv";
        if (strcmp(last_dot, ".gif") == 0) return "image/gif";
        if (strcmp(last_dot, ".htm") == 0) return "text/html";
        if (strcmp(last_dot, ".html") == 0) return "text/html";
        if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
        if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".js") == 0) return "application/javascript";
        if (strcmp(last_dot, ".json") == 0) return "application/json";
        if (strcmp(last_dot, ".png") == 0) return "image/png";
        if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
        if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
        if (strcmp(last_dot, ".txt") == 0) return "text/plain";
        if (strcmp(last_dot, ".mp4") == 0) return "video/mp4";
    }

    return "application/octet-stream";
}


int responseClient::checkUri(std::string _uri)
{
    struct stat info;

    // std::cout << "********** =>>> path = [" << _uri << "] <<<= **********\n" << std::endl;
    if (stat(_uri.c_str(), &info) != 0)
    {
        // std::cout << "path not valid" << std::endl;   
       return send_error_status("404");
    }
    else
    {
        // std::cout << "path valid !" << std::endl;
        client->flagRespose->file_RW.open(_uri);
        if (!client->flagRespose->file_RW)
        {
            // std::cout << " _uri not file !  " <<  _uri  << std::endl;
            client->flagRespose->file_RW.open(_uri + index);
            if (!client->flagRespose->file_RW)
            {
                // std::cout << " file not valid !  " <<  _uri + index << std::endl;
                return send_error_status("404");
            }
            uri = _uri + index;
        }
        else
            uri = _uri;
    }
    return 0;
}
 
int responseClient::getMethode(responseClient& cl)
{
    cl.content_type = get_content_type(cl.uri.c_str());
    cl.client->flagRespose->file_RW.seekg(0, std::ios::end);
    cl.client->flagRespose->content_lenght = cl.client->flagRespose->file_RW.tellg();
    cl.client->flagRespose->file_RW.seekg(0,std::ios::beg);
    // std::cout << "data " << cl.client->flagRespose->content_lenght << std::endl;
    cl.buff << "HTTP/1.1 " << cl.statusCodes["200"] \
                << "\r\nContent-Type: "  << cl.content_type\
                << "\r\nContent-Length: " << cl.client->flagRespose->content_lenght \
                << "\r\nConnection: close\r\n"\
                << "\r\n";

            write(cl.client->socket, cl.buff.str().c_str(), cl.buff.str().length());
    // std::cout << "*-*-*-*-*-*-*-* GET methode *-*-*-*-*-*-*-*" << std::endl;
    // cl.send_data();
    return 0;
}

int responseClient::postMethode(responseClient& cl)
{
    std::cout << "POSTE methode" << std::endl;
    std::cout << "CLient " << cl.client->request_data_struct->body << std::endl;
    char test_response[] = "HTTP/1.1 200 OK\r\nContent-Length: 20\r\n\r\nOK hamid khdamaaaa!!";
            write (cl.client->socket, (char *)test_response, sizeof(test_response));  
    return 0;
}

int responseClient::deleteMethode(responseClient& cl)
{
    (void)cl;
    std::cout << "DELETE methode" << std::endl;
    return 0;
}



responseClient::responseClient(std::vector<config_file> &blockServer):block_server(blockServer)
{
    // initial methode Function
    methodeFunction["GET"] = getMethode;
    methodeFunction["POST"] = postMethode;
    methodeFunction["DELETE"] = deleteMethode;
    // initial status code 
    statusCodes["100"] = "100 Continue";
    statusCodes["101"] = "101 Switching Protocols";
    statusCodes["102"] = "102 Processing";
    statusCodes["103"] = "103 Early Hints";
    statusCodes["200"] = "200 OK";
    statusCodes["201"] = "201 Created";
    statusCodes["202"] = "202 Accepted";
    statusCodes["203"] = "203 Non-Authoritative Information";
    statusCodes["204"] = "204 No Content";
    statusCodes["205"] = "205 Reset Content";
    statusCodes["206"] = "206 Partial Content";
    statusCodes["207"] = "207 Multi-Status";
    statusCodes["208"] = "208 Already Reported";
    statusCodes["226"] = "226 IM Used";
    statusCodes["300"] = "300 Multiple Choices";
    statusCodes["301"] = "301 Moved Permanently";
    statusCodes["302"] = "302 Found";
    statusCodes["303"] = "303 See Other";
    statusCodes["304"] = "304 Not Modified";
    statusCodes["305"] = "305 Use Proxy";
    statusCodes["306"] = "306 Switch Proxy";
    statusCodes["307"] = "307 Temporary Redirect";
    statusCodes["308"] = "308 Permanent Redirect";
    statusCodes["400"] = "400 Bad Request";
    statusCodes["401"] = "401 Unauthorized";
    statusCodes["402"] = "402 Payment Required";
    statusCodes["403"] = "403 Forbidden";
    statusCodes["404"] = "404 Not Found";
    statusCodes["405"] = "405 Method Not Allowed";
    statusCodes["406"] = "406 Not Acceptable";
    statusCodes["407"] = "407 Proxy Authentication Required";
    statusCodes["408"] = "408 Request Timeout";
    statusCodes["409"] = "409 Conflict";
    statusCodes["410"] = "410 Gone";
    statusCodes["411"] = "411 Length Required";
    statusCodes["412"] = "412 Precondition Failed";
    statusCodes["413"] = "413 Payload Too Large";
    statusCodes["414"] = "414 URI Too Long";
    statusCodes["415"] = "415 Unsupported Media Type";
    statusCodes["416"] = "416 Range Not Satisfiable";
    statusCodes["417"] = "417 Expectation Failed";
    statusCodes["418"] = "418 I'm a teapot";
    statusCodes["421"] = "421 Misdirected Request";
    statusCodes["422"] = "422 Unprocessable Entity";
    statusCodes["423"] = "423 Locked";
    statusCodes["424"] = "424 Failed Dependency";
    statusCodes["425"] = "425 Too Early";
    statusCodes["426"] = "426 Upgrade Required";
    statusCodes["428"] = "428 Precondition Required";
    statusCodes["429"] = "429 Too Many Requests";
    statusCodes["431"] = "431 Request Header Fields Too Large";
    statusCodes["451"] = "451 Unavailable For Legal Reasons";
    statusCodes["500"] = "500 Internal Server Error";
    statusCodes["501"] = "501 Not Implemented";
    statusCodes["502"] = "502 Bad Gateway";
    statusCodes["503"] = "503 Service Unavailable";
    statusCodes["504"] = "504 Gateway Timeout";
    statusCodes["505"] = "505 HTTP Version Not Supported";
    statusCodes["506"] = "506 Variant Also Negotiates";
    statusCodes["507"] = "507 Insufficient Storage";
    statusCodes["508"] = "508 Loop Detected";
    statusCodes["510"] = "510 Not Extended";
    statusCodes["511"] = "511 Network Authentication Required";
}

int responseClient::root_directory_if_existe()
{
    if (!root.compare(0, 2, "./") || !root.compare(0, 3, "../"))
    {
        char tmp[256];
        std::string currentDe(getcwd(tmp, 256));
        if (root[root.length() - 1] != '/')
            root = root + "/";
        root = currentDe + "/" + root;
    }
    else if (root[0] != '/')    
        root = "/" + root;
        //  std::cout << "here "<< client->request_data_struct->path << "\n\n "<< std::endl;
    nbrstatus = checkUri(root + client->request_data_struct->path.substr(1, client->request_data_struct->path.length()));
    return nbrstatus;
}

int     responseClient::noLocation()
{
    
    std::cout << "----------- No location ---------" << std::endl;
    //check if methode allowed
    std::string MetAllowd("GET POST DELETE");

    if ((block_server[nBlock].allow_method.length() > 0 && block_server[nBlock].allow_method.find(client->request_data_struct->method) == std::string::npos)
    || MetAllowd.find(client->request_data_struct->method) == std::string::npos)
    {
        std::cout << "method Not Allowed" << std::endl;
        return send_error_status("405");
    }
    return 0;
}

int responseClient::check_if_location_matched()
{
    size_t lenPathLocation = block_server[nBlock].list_of_location[0].path.length();
    int i = 0;
    std::string rootLocation;
    for(size_t nLocation = 0; nLocation < block_server[nBlock].list_of_location.size(); nLocation++)
    {
        
        if (client->request_data_struct->path.find(block_server[nBlock].list_of_location[nLocation].path) != std::string::npos \
            && (client->request_data_struct->path[block_server[nBlock].list_of_location[nLocation].path.length()] == '\0' || \
            client->request_data_struct->path[block_server[nBlock].list_of_location[nLocation].path.length()] == '/'))
        {
            std::cout << "------------------------- location is matched -------------------------\n" << std::endl;
            if (!i++ || (block_server[nBlock].list_of_location.size() > 1 && nLocation > 0 && lenPathLocation <= block_server[nBlock].list_of_location[nLocation].path.length()))
            { 
                if (block_server[nBlock].list_of_location[nLocation].index[0] == '/')
                    index = block_server[nBlock].list_of_location[nLocation].index.substr(1);
                else
                    index = block_server[nBlock].list_of_location[nLocation].index;
                rootLocation = client->request_data_struct->path.substr(block_server[nBlock].list_of_location[nLocation].path.length()\
                            , client->request_data_struct->path.length());
            }
        }
    }
    if (i)
        client->request_data_struct->path = rootLocation.length() == 0 ? "/" : rootLocation ;
    return 0;
}

int responseClient::ft_response()
{
    // std::cout << "*-*-*-*-*-*-*-*-*--*-*-*-*-*-*----------*-*-* start respose *-*-*-*-*-*-*-*-*--*-*-*-*-*-*----------*-*-* " << std::endl;
    if (client->flagRespose->isReading)
    {
        // std::cout << "sending data" << std::endl;
        send_data();
    }
    else
    {
        client->flagRespose->isReading = true;
         /// check method if not allowed
        if (client->request_data_struct->method.empty())
            return send_error_status("405");

        ///#### check server block
        int sfind = client->request_data_struct->host.find(":");
        std::string host_serv = client->request_data_struct->host.substr(0, sfind);
        std::string port_serv = client->request_data_struct->host.substr(sfind + 1, client->request_data_struct->host.length());
        nbrstatus = 0;
        /// check Block Server
    
        for(size_t nServer=0; nServer < block_server.size() - 1; nServer++)
        {
            
        
            if ((!block_server[nServer].server_name.compare(host_serv) || !block_server[nServer].server.compare(host_serv)) \
                && !block_server[nServer].port.compare(port_serv))
            {
                root = block_server[nServer].root;
                index = block_server[nServer].index;
                nBlock = nServer;
                //check if methode allowed
                std::string MetAllowd("GET POST DELETE");
            
                if ((block_server[nBlock].allow_method.length() > 0 && block_server[nBlock].allow_method.find(client->request_data_struct->method) == std::string::npos)
                || MetAllowd.find(client->request_data_struct->method) == std::string::npos)
                {
                    std::cout << "method Not Allowed" << std::endl;
                    return send_error_status("405");
                }
                //#### matching location
                if (block_server[nServer].list_of_location.size() == 0)
                    noLocation();
                else
                    check_if_location_matched();
                /// get current root Directory 
                if (root_directory_if_existe())
                    return nbrstatus;

                /// calling Methode needed
                methodeFunction[client->request_data_struct->method](*this);
                return nbrstatus;
            }
            // else
            // {
                 /////////////// for testing ......

                // char test_response[] = "HTTP/1.1 200 OK\r\nContent-Length: 20\r\n\r\nOK hamid khdamaaaa!!";
                // write (client->socket, (char *)test_response, sizeof(test_response));
                // client->flagRespose->isReading = false;
                ///////////////
                // return 0;
            // }

        }
    }
    
    return 0;
}