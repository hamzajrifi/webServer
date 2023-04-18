#include "response.hpp"

int responseClient::nbrDataUpload = 0;

const char *responseClient::get_content_type(const char* path) 
{
    const char *last_dot = strrchr(path, '.');

    if (last_dot) {
        if (strcmp(last_dot,".php" ) == 0) return "text/html";
        if (strcmp(last_dot,".csv" ) == 0) return "text/csv";
        if (strcmp(last_dot,".gif" ) == 0) return "image/gif";
        if (strcmp(last_dot,".htm" ) == 0) return "text/html";
        if (strcmp(last_dot,".html") == 0) return "text/html";
        if (strcmp(last_dot,".ico" ) == 0) return "image/x-icon";
        if (strcmp(last_dot,".jpeg") == 0) return "image/jpeg";
        if (strcmp(last_dot,".jpg" ) == 0) return "image/jpeg";
        if (strcmp(last_dot,".js"  ) == 0) return "application/javascript";
        if (strcmp(last_dot,".json") == 0) return "application/json";
        if (strcmp(last_dot,".pdf" ) == 0) return "application/pdf";
        if (strcmp(last_dot,".png" ) == 0) return "image/png";
        if (strcmp(last_dot,".svg" ) == 0) return "image/svg+xml";
        if (strcmp(last_dot,".txt" ) == 0) return "text/plain";
        if (strcmp(last_dot,".mp4" ) == 0) return "video/mp4";
        if (strcmp(last_dot,".WebM") == 0) return "video/webm";
        if (strcmp(last_dot,".Ogg" ) == 0) return "video/ogg";
        if (strcmp(last_dot,".AVI" ) == 0) return "video/x-msvideo";
        if (strcmp(last_dot,".MPEG") == 0) return "video/mpeg";
        if (strcmp(last_dot,".tiff") == 0) return "image/tiff";
        if (strcmp(last_dot,".tif" ) == 0) return "image/tiff";
        if (strcmp(last_dot,".xml" ) == 0) return "application/xml";
        if (strcmp(last_dot,".zip" ) == 0) return "application/zip";
        if (strcmp(last_dot,".gz"  ) == 0) return "application/gzip";
        if (strcmp(last_dot,".tar" ) == 0) return "application/x-tar";
        if (strcmp(last_dot,".rar" ) == 0) return "application/x-rar-compressed";
        if (strcmp(last_dot,".7z"  ) == 0) return "application/x-7z-compressed";
        if (strcmp(last_dot,".mp3" ) == 0) return "audio/mpeg";
        if (strcmp(last_dot,".wav" ) == 0) return "audio/wav";
        if (strcmp(last_dot,".ogg" ) == 0) return "audio/ogg";
        if (strcmp(last_dot,".flac") == 0) return "audio/flac";
        if (strcmp(last_dot,".aac" ) == 0) return "audio/aac";
        if (strcmp(last_dot,".mpga") == 0) return "audio/mpeg";
        if (strcmp(last_dot,".mid" ) == 0) return "audio/midi";
        if (strcmp(last_dot,".midi") == 0) return "audio/midi";
        if (strcmp(last_dot,".ppt" ) == 0) return "application/vnd.ms-powerpoint";
        if (strcmp(last_dot,".pptx") == 0) return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
        if (strcmp(last_dot,".xls" ) == 0) return "application/vnd.ms-excel";
        if (strcmp(last_dot,".xlsx") == 0) return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
        if (strcmp(last_dot,".doc" ) == 0) return "application/msword";
        if (strcmp(last_dot,".docx") == 0) return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
        if (client->flagResponse->ifautoIndex) return "text/html";
    }

    return "application/octet-stream";
}

const char *get_lastEnd_content_type(const char* type) 
{
    if (type) {
        if (strcmp(type, "text/css") == 0) return ".css";
        if (strcmp(type, "text/csv") == 0) return ".csv";
        if (strcmp(type, "image/gif") == 0) return ".gif";
        if (strcmp(type, "text/html") == 0) return ".htm";
        if (strcmp(type, "image/x-icon") == 0) return ".ico";
        if (strcmp(type, "image/jpeg") == 0) return ".jpg";
        if (strcmp(type, "image/png") == 0) return ".png";
        if (strcmp(type, "image/svg+xml") == 0) return ".svg";
        if (strcmp(type, "video/mp4") == 0) return ".mp4";
        if (strcmp(type, "text/plain") == 0) return ".txt";
        if (strcmp(type, "application/pdf") == 0) return ".pdf";
        if (strcmp(type, "image/jpeg") == 0) return ".jpeg" ;
        if (strcmp(type, "application/json") == 0) return ".json" ;
        if (strcmp(type, "application/javascript") == 0) return  ".js";
    }

    return ".bin";
}

size_t	responseClient::list_current_directory(std::string _uri)
{
    DIR* dirp;
    struct dirent* direntp;
    
    dirp = opendir( _uri.c_str() );
    if( dirp == NULL ) {
        // perror( "can't open /home/fred" );
    } else {
        buff2 << "<!DOCTYPE html> \
                <html> \
                    <head><style>h1 {text-align: center;}</style></head> \
                    <body> \
                        <h1 id=\"header\">Index of LOCATION</h1> \
                        <div id=\"parentDirLinkBox\" style=\"display:none\"> \
                          <a id=\"parentDirLink\" class=\"icon up\"> \
                            <span id=\"parentDirText\">[parent directory]</span> \
                          </a> \
                        </div> \
                        <table> \
                            <thead> \
                                <tr class=\"header\" id=\"theader\"> \
                                    <th id=\"nameColumnHeader\" tabindex=0 role=\"button\">Name</th> \
                                    <th id=\"sizeColumnHeader\" class=\"detailsColumn\" tabindex=0 role=\"button\"> \
                                      Size \
                                    </th> \
                                    <th id=\"dateColumnHeader\" class=\"detailsColumn\" tabindex=0 role=\"button\"> \
                                      Date Modified \
                                    </th> \
                                </tr> \
                            </thead> \
                            <tbody id=\"tbody\"> ";
        for(;;) {
            direntp = readdir( dirp );
            if( direntp == NULL ) break;
            buff2 << "<tr><td data-value=\"" << direntp->d_name << "\"><a class=\"icon file\" draggable=\"true\" href=\"" << \
                        (client->request_data_struct->path.length() == 1 ? "/" : client->request_data_struct->path + "/") << direntp->d_name << "\">" << direntp->d_name << "</a></td></rtr>";
            // printf( "%s\n", direntp->d_name );
        }
            buff2 << "</tbody></table> </body> </html> ";
        closedir( dirp );
        client->flagResponse->content_length  = buff2.str().length();
        return 403;
    }
    return 0;
}

int responseClient::checkUri(std::string _uri)
{
    struct stat info;

    // std::cout << "********** =>>> path = [" << _uri << "] <<<= **********\n" << std::endl;
    if (stat(_uri.c_str(), &info) != 0)
    {
        std::cout << "path not valid" << std::endl;   
       return send_error_status("404");
    }
    else
    {
        std::cout << "path valid !" << std::endl;
        client->flagResponse->file_RW.open(_uri);
        if (!client->flagResponse->file_RW)
        {
            std::cout << " _uri not file !  " <<  _uri  << std::endl;
            if (client->flagResponse->ifautoIndex)
            {
                std::cout << "inside uri " << std::endl;
                uri = _uri;
                return 0;
            } //list_current_directory(_uri);
                // return send_error_status("403");
            if (!client->request_data_struct->method.find("DELETE"))
                return send_error_status("409");

            if (!client->flagResponse->isLocation 
                && client->request_data_struct->path.length() > 1 
                && block_server[nBlock].index[0] != '/')
                    index = "/" + index;

            client->flagResponse->file_RW.open(_uri + index);
            if (!client->flagResponse->file_RW)
            {
                std::cout << " file not valid !  " <<  _uri + index << std::endl;
                return send_error_status("404");
            }
            uri = _uri + index;
        }
        else
            uri = _uri;
    }
    return 0;
}
 
int responseClient::getMethod(responseClient& cl)
{
    cl.content_type = cl.get_content_type(cl.uri.c_str());
    cl.client->flagResponse->file_RW.seekg(0, std::ios::end);
    cl.client->flagResponse->content_length = cl.client->flagResponse->file_RW.tellg();
    cl.client->flagResponse->file_RW.seekg(0,std::ios::beg);
    // cl.client->flagResponse->content_length = cl.client->flagResponse->ifautoIndex ? cl.list_current_directory(cl.uri) : cl.client->flagResponse->content_length;
    if (cl.client->flagResponse->ifautoIndex)
        cl.list_current_directory(cl.uri);
    std::cout << "data " << cl.client->flagResponse->content_length << std::endl;
    cl.buff << "HTTP/1.1 " << cl.statusCodes["200"] \
                << "\r\nContent-Type: "  << cl.content_type\
                << "\r\nContent-Length: " << cl.client->flagResponse->content_length \
                << "\r\nConnection: close\r\n"\
                << "\r\n";

    if (write(cl.client->socket, cl.buff.str().c_str(), cl.buff.str().length()) < 0)
        std::cout << "write fieled222222222 " << std::endl;
    if (cl.client->flagResponse->ifautoIndex)
        if (write(cl.client->socket, cl.buff2.str().c_str(), cl.buff2.str().length()) < 0)
            std::cout << "write fieled11111 " << std::endl;

    // std::cout << "*-*-*-*-*-*-*-* GET methode *-*-*-*-*-*-*-*" << std::endl;
    return 0;
}

int responseClient::postMethod(responseClient& cl)
{
    std::cout << "*-*-*-*-*-*-*-* POST method *-*-*-*-*-*-*-*" << std::endl;
        // std::cout << "data " << cl.client->request_data_struct->body << std::endl;
    ////check if body is empty
    // if (!cl.client->request_data_struct->content_Type.length())
    // {
        // std::cout  << "POST empty " << std::endl;
        cl.buff << "HTTP/1.1 " << cl.statusCodes["201"] \
                << "\r\nContent-Type: "  << "text/plain"\
                << "\r\nContent-Length: " << "7" \
                << "\r\nConnection: close\r\n"\
                << "\r\nCreated";
        write(cl.client->socket, cl.buff.str().c_str(), cl.buff.str().length());
        cl.client->flagResponse->isReading = false;
    // 	return 201;
    // }
    //// check data conetent type request and Create and open a file
    std::stringstream nameFile ;
    nameFile <<  "../upload/fd_app_" << nbrDataUpload << get_lastEnd_content_type(cl.client->request_data_struct->content_Type.c_str());
      std::ofstream PostFile(nameFile.str());
    nbrDataUpload++;
      // Write to the file
      PostFile << cl.client->request_data_struct->body;
      PostFile.close();
        drop_client(cl.client);
    cl.client->flagResponse->isReading = false;
    return 0;
}

int responseClient::deleteMethod(responseClient& cl)
{
    std::cout << "DELETE methode" << " \n URI = " << cl.client->request_data_struct->path << std::endl;
    cl.client->request_data_struct->path = "../" + cl.client->request_data_struct->path;
    if (remove(cl.client->request_data_struct->path.c_str()))
        std::cout << "can't remove " << std::endl;
    cl.buff << "HTTP/1.1 " << cl.statusCodes["202"] \
                << "\r\nContent-Type: "  << "text/plain"\
                << "\r\nContent-Length: " << "7" \
                << "\r\nConnection: close\r\n"\
                << "\r\nDeleted";
        write(cl.client->socket, cl.buff.str().c_str(), cl.buff.str().length());
        cl.client->flagResponse->isReading = false;
    return 0;
}

int	responseClient::check_method()
{
    //check if methode allowed
    std::string MetAllowd("GET POST DELETE");
    if (client->request_data_struct->method.empty() || MetAllowd.find(client->request_data_struct->method) == std::string::npos)
    {
        client->request_data_struct->nbrStatus = "501";
        return 501;
    }
    else if (client->flagResponse->isLocation
            && !block_server[nBlock].list_of_location[nLocation].allow_method.empty())
    {
        client->flagResponse->allowedMethod = (block_server[nBlock].list_of_location[nLocation].allow_method.find("POST") == std::string::npos
        && block_server[nBlock].list_of_location[nLocation].allow_method.find("post") == std::string::npos) ? " " : "POST";

        client->flagResponse->allowedMethod += (block_server[nBlock].list_of_location[nLocation].allow_method.find("GET") == std::string::npos
        && block_server[nBlock].list_of_location[nLocation].allow_method.find("get") == std::string::npos) ? " " : " GET";

        client->flagResponse->allowedMethod += (block_server[nBlock].list_of_location[nLocation].allow_method.find("DELETE") == std::string::npos
        && block_server[nBlock].list_of_location[nLocation].allow_method.find("delete") == std::string::npos) ? " " : " DELETE";
    
        if (client->flagResponse->allowedMethod.find(client->request_data_struct->method) == std::string::npos)
        {
            client->request_data_struct->nbrStatus = "405";
            return 405;
        }
    }
    return 0;
}

responseClient::responseClient(std::vector<config_file> &blockServer):block_server(blockServer)
{
    // initial methode Function
    methodeFunction["GET"] = getMethod;
    methodeFunction["POST"] = postMethod;
    methodeFunction["DELETE"] = deleteMethod;
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
         std::cout << "request path "<< client->request_data_struct->path << "\n "<< std::endl;
    nbrstatus = checkUri(root + client->request_data_struct->path.substr(1, client->request_data_struct->path.length()));
    return nbrstatus;
}

int	responseClient::noLocation()
{
    std::cout << "----------- No location ---------" << std::endl;
    if (block_server[nBlock].index[0] == '/')
        index = block_server[nBlock].list_of_location[nLocation].index.substr(1);
    return 0;
}

int responseClient::check_if_location_matched()
{
    size_t lenPathLocation = block_server[nBlock].list_of_location[0].path.length();
    int i = 0;
    std::string rootLocation;
    for(nLocation = 0; nLocation < block_server[nBlock].list_of_location.size(); nLocation++)
    {
        // std::cout << "nblock =  " << nLocation << " nlocation " << block_server[nBlock].list_of_location[nLocation].path \
        // << "path " <<  client->request_data_struct->path << std::endl;
        if (client->request_data_struct->path.find(block_server[nBlock].list_of_location[nLocation].path) != std::string::npos \
            && (client->request_data_struct->path[block_server[nBlock].list_of_location[nLocation].path.length()] == '\0' || \
            client->request_data_struct->path[block_server[nBlock].list_of_location[nLocation].path.length()] == '/'))
        {
            std::cout << "------------------------- location is matched -------------------------\n" << std::endl;
            client->flagResponse->isLocation = true;
            if (!i++ || (block_server[nBlock].list_of_location.size() > 1 && nLocation > 0 
            && lenPathLocation <= block_server[nBlock].list_of_location[nLocation].path.length()))
            {
                if (!block_server[nBlock].list_of_location[nLocation].allow_method.empty() && check_method())
                    return send_error_status("405");
                
                if (!block_server[nBlock].list_of_location[nLocation].autoindex.find("on"))
                    client->flagResponse->ifautoIndex = true;
                if (!block_server[nBlock].list_of_location[nLocation].returno.empty())
                {
                    index = block_server[nBlock].list_of_location[nLocation].returno;
                    return send_error_status("301");
                }
                    // std::cout << "root " << block_server[nBlock].list_of_location[nLocation].returno << std::endl;
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
    ///check if client is conected
    if (client->flagResponse->isReading)
        send_data();
    ///check if error in request
    else if (!client->request_data_struct->nbrStatus.empty() || check_method())
        return (send_error_status(client->request_data_struct->nbrStatus.c_str()));
    else
    {
        client->flagResponse->isReading = true;
        ///#### check server block
        int sfind = client->request_data_struct->host.find(":");
        std::string host_serv = client->request_data_struct->host.substr(0, sfind);
        std::string port_serv = client->request_data_struct->host.substr(sfind + 1, client->request_data_struct->host.length());
        nbrstatus = 0;
        /// check Block Server
        noServerMatched = false;
        for(size_t nServer=0; nServer < block_server.size() - 1; nServer++)
        {
            std::cout << "size body : " << block_server[nServer].max_number << std::endl;
            if (!client->request_data_struct->method.find("POST") && client->request_data_struct->body.size() > block_server[nServer].max_number)
                return send_error_status("413");
            if (((!block_server[nServer].server_name.compare(host_serv) || !block_server[nServer].server.compare(host_serv)) \
                && !block_server[nServer].port.compare(port_serv)) 
                || noServerMatched)
            {
                root = block_server[nServer].root;
                index = block_server[nServer].index;
                nBlock = nServer;
                //#### matching location
                if (block_server[nServer].list_of_location.size() == 0)
                    noLocation();
                else if (check_if_location_matched())
                    return nbrstatus;
                    
                /// get current root Directory
                if (root_directory_if_existe())
                    return nbrstatus;

                // if (client.ifNeedCgi)
                    return handle_cgi();
                    
                /// calling Methode needed
                //methodeFunction[client->request_data_struct->method](*this);
                return nbrstatus;
            }
            else if (nServer == block_server.size() - 2)
            {
                nServer = -1;
                noServerMatched = true;
            }
        }
    }
    return 0;
}