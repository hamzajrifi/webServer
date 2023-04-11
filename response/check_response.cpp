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
    }

    return "application/octet-stream";
}


int responceClient::checkUri(std::string uri)
{
    struct stat info;

    std::cout << "path = " << uri << std::endl;
    if (stat(uri.c_str(), &info) != 0)
        send_404();
    else
    {
        // if (block_server[nBlock].index)    
        std::cout << block_server[nBlock].index << std::endl;
    }

    return 0;
}

int responceClient::getMethode(std::string methode)
{
    std::cout << methode << std::endl;
    return 0;
}

int responceClient::postMethode(std::string methode)
{
    std::cout << methode << std::endl;
    return 0;
}

int responceClient::deleteMethode(std::string methode)
{
    std::cout << methode << std::endl;
    return 0;
}

responceClient::responceClient(std::vector<config_file> &blockServer):block_server(blockServer)
{
    // initial methode Function
    methodeFunction["GET"] = getMethode;
    methodeFunction["POSTE"] = postMethode;
    methodeFunction["DELETE"] = deleteMethode;
    // initial status code 
    statusCodes[100] = "100 Continue";
    statusCodes[101] = "101 Switching Protocols";
    statusCodes[102] = "102 Processing";
    statusCodes[103] = "103 Early Hints";
    statusCodes[200] = "200 OK";
    statusCodes[201] = "201 Created";
    statusCodes[202] = "202 Accepted";
    statusCodes[203] = "203 Non-Authoritative Information";
    statusCodes[204] = "204 No Content";
    statusCodes[205] = "205 Reset Content";
    statusCodes[206] = "206 Partial Content";
    statusCodes[207] = "207 Multi-Status";
    statusCodes[208] = "208 Already Reported";
    statusCodes[226] = "226 IM Used";
    statusCodes[300] = "300 Multiple Choices";
    statusCodes[301] = "301 Moved Permanently";
    statusCodes[302] = "302 Found";
    statusCodes[303] = "303 See Other";
    statusCodes[304] = "304 Not Modified";
    statusCodes[305] = "305 Use Proxy";
    statusCodes[306] = "306 Switch Proxy";
    statusCodes[307] = "307 Temporary Redirect";
    statusCodes[308] = "308 Permanent Redirect";
    statusCodes[400] = "400 Bad Request";
    statusCodes[401] = "401 Unauthorized";
    statusCodes[402] = "402 Payment Required";
    statusCodes[403] = "403 Forbidden";
    statusCodes[404] = "404 Not Found";
    statusCodes[405] = "405 Method Not Allowed";
    statusCodes[406] = "406 Not Acceptable";
    statusCodes[407] = "407 Proxy Authentication Required";
    statusCodes[408] = "408 Request Timeout";
    statusCodes[409] = "409 Conflict";
    statusCodes[410] = "410 Gone";
    statusCodes[411] = "411 Length Required";
    statusCodes[412] = "412 Precondition Failed";
    statusCodes[413] = "413 Payload Too Large";
    statusCodes[414] = "414 URI Too Long";
    statusCodes[415] = "415 Unsupported Media Type";
    statusCodes[416] = "416 Range Not Satisfiable";
    statusCodes[417] = "417 Expectation Failed";
    statusCodes[418] = "418 I'm a teapot";
    statusCodes[421] = "421 Misdirected Request";
    statusCodes[422] = "422 Unprocessable Entity";
    statusCodes[423] = "423 Locked";
    statusCodes[424] = "424 Failed Dependency";
    statusCodes[425] = "425 Too Early";
    statusCodes[426] = "426 Upgrade Required";
    statusCodes[428] = "428 Precondition Required";
    statusCodes[429] = "429 Too Many Requests";
    statusCodes[431] = "431 Request Header Fields Too Large";
    statusCodes[451] = "451 Unavailable For Legal Reasons";
    statusCodes[500] = "500 Internal Server Error";
    statusCodes[501] = "501 Not Implemented";
    statusCodes[502] = "502 Bad Gateway";
    statusCodes[503] = "503 Service Unavailable";
    statusCodes[504] = "504 Gateway Timeout";
    statusCodes[505] = "505 HTTP Version Not Supported";
    statusCodes[506] = "506 Variant Also Negotiates";
    statusCodes[507] = "507 Insufficient Storage";
    statusCodes[508] = "508 Loop Detected";
    statusCodes[510] = "510 Not Extended";
    statusCodes[511] = "511 Network Authentication Required";
}

int     responceClient::noLocation()
{
    std::cout << "no location" << std::endl;
    /// get current Directory 
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
    nbrstatus = checkUri(root + client->request_data_struct->path.substr(1, client->request_data_struct->path.length()));
    return 0;
}

int responceClient::ft_response(responceClient *res_client)
{
    // if (MAX_REQUEST_SIZE == res_client->client->received)
        // return 400;
    ///#### check server block
    int sfind = client->request_data_struct->host.find(":");
    std::string host_serv = client->request_data_struct->host.substr(0, sfind);
    std::string port_serv = client->request_data_struct->host.substr(sfind + 1, client->request_data_struct->host.length());
    std::cout << "check block server " << std::endl;
    for(size_t i=0; i < block_server.size() - 1; i++){
        if (!block_server[i].server.compare(host_serv) && !block_server[i].port.compare(port_serv))
        {   
            root = block_server[i].root;
            // index = block_server[0].index;
            //#### matching location
            // client->nBlock = i;
            if (block_server[i].list_of_location.size() == 0)
                return  noLocation();

            // for(size_t j=0; j < block_server[i].list_of_location.size() - 1; i++)
            // {
            //     // int k = 0;
            //     // while (block_server[i].list_of_location[j].path[k] == client->request_data_struct->path[k])
            //     // {}
            // }
            //// methode 
            methodeFunction[res_client->client->request_data_struct->method]("TEST");

            char test_response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nhave location";
            write (client->socket, (char *)test_response, sizeof(test_response));
            /// don't forget to check protocol version !!!!!!
            // sprintf(buffer_response, "HTTP/1.1 %s\r\nContent-Length: %u\r\nContent-Type: %s\r\n\r\n%s",\
            //         "ok", contenet_lenght, content_type.c_str(), "hjrifi");
            // write(res_client->client->socket, (char *)buffer_response, strlen(buffer_response));        
            return 0;
        }
    }
    return 0;
}