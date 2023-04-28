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
    }
        if (client->flagResponse->ifautoIndex) return "text/html";

    return "application/octet-stream";
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
                        ((client->request_data_struct->path.length() == 1 && !client->flagResponse->isLocation) ? "/" : \

                            client->flagResponse->isLocation ? (locationMatched + client->request_data_struct->path + "/") :\

                                client->request_data_struct->path)\
                                
                    << direntp->d_name << "\">" << direntp->d_name << "</a></td></rtr>";
        }
            buff2 << "</tbody></table> </body> </html> ";
        closedir( dirp );
        std::cout << ">> " << locationMatched  << "--" << client->request_data_struct->path << std::endl;
        client->flagResponse->content_length  = buff2.str().length();
        return 403;
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
        time (&rawtime);
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
            //std::cout << "size body : " << block_server[nServer].max_number << std::endl;
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
                std::cout << "last uri " << uri << std::endl;
                if (root_directory_if_existe())
                    return nbrstatus;
                
                std::cout << "last uri " << uri << std::endl;
                /// get current root Directory
                client->flagResponse->file_RW.close();
                client->flagResponse->file_RW.open(uri);
                if (client->flagResponse->isLocation && !block_server[nBlock].list_of_location[nbrLocation].cgi_path.empty() 
                    && client->flagResponse->file_RW)
                {
                    std::cout << "----- CGI -----" << std::endl;
                    handle_cgi();
                }
                /// calling Methode needed
                else
                {
                    methodeFunction[client->request_data_struct->method](*this);
                }

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