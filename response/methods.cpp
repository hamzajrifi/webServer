#include "response.hpp"

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
        if (strcmp(type, "plain/text") == 0) return ".txt";
        if (strcmp(type, "application/pdf") == 0) return ".pdf";
        if (strcmp(type, "image/jpeg") == 0) return ".jpeg" ;
        if (strcmp(type, "application/json") == 0) return ".json" ;
        if (strcmp(type, "application/javascript") == 0) return  ".js";
    }

    return ".bin";
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
            && !block_server[nBlock].list_of_location[nbrLocation].allow_method.empty())
    {
        client->flagResponse->allowedMethod = (block_server[nBlock].list_of_location[nbrLocation].allow_method.find("POST") == std::string::npos
        && block_server[nBlock].list_of_location[nbrLocation].allow_method.find("post") == std::string::npos) ? " " : "POST";

        client->flagResponse->allowedMethod += (block_server[nBlock].list_of_location[nbrLocation].allow_method.find("GET") == std::string::npos
        && block_server[nBlock].list_of_location[nbrLocation].allow_method.find("get") == std::string::npos) ? " " : " GET";

        client->flagResponse->allowedMethod += (block_server[nBlock].list_of_location[nbrLocation].allow_method.find("DELETE") == std::string::npos
        && block_server[nBlock].list_of_location[nbrLocation].allow_method.find("delete") == std::string::npos) ? " " : " DELETE";
    
        if (client->flagResponse->allowedMethod.find(client->request_data_struct->method) == std::string::npos)
        {
            client->request_data_struct->nbrStatus = "405";
            return 405;
        }
    }
    return 0;
}

int responseClient::getMethod(responseClient& cl)
{
    cl.content_type = cl.get_content_type(cl.uri.c_str());
    cl.client->flagResponse->file_RW.seekg(0, std::ios::end);
    cl.client->flagResponse->content_length = cl.client->flagResponse->file_RW.tellg();
    cl.client->flagResponse->file_RW.seekg(0,std::ios::beg);
    if (cl.client->flagResponse->ifautoIndex)
        cl.list_current_directory(cl.uri);
    cl.buff << "HTTP/1.1 " << cl.statusCodes["200"] \
                << "\r\nContent-Type: "  << cl.content_type\
                << "\r\nContent-Length: " << cl.client->flagResponse->content_length \
                << "\r\nConnection: close\r\n"\
                << "\r\n";

    if (cl.sendHeader(cl.client->socket, cl.buff.str().c_str(), cl.buff.str().length()))
            return -1;
    if (cl.client->flagResponse->ifautoIndex)
        if (cl.sendHeader(cl.client->socket, cl.buff2.str().c_str(), cl.buff2.str().length()))
            return -1;
    return 0;
}

int responseClient::postMethod(responseClient& cl)
{
    char ptr[256];
    cl.tmp_string = getcwd(ptr, 256);
    if (cl.block_server[cl.nBlock].upload_file.empty())
        cl.block_server[cl.nBlock].upload_file = cl.tmp_string + "/public/upload";
    else
        cl.block_server[cl.nBlock].upload_file = cl.tmp_string + "/" + cl.block_server[cl.nBlock].upload_file + "/";
    if (stat((cl.block_server[cl.nBlock].upload_file).c_str(), &cl.info) != 0)
        return cl.send_error_status("500");
        cl.buff << "HTTP/1.1 " << cl.statusCodes["201"] \
                << "\r\nContent-Type: "  << "text/plain"\
                << "\r\nContent-Length: " << "7" \
                << "\r\nConnection: close\r\n"\
                << "\r\nCreated";
    if (cl.sendHeader(cl.client->socket, cl.buff.str().c_str(), cl.buff.str().length()))
        return -1;

    //// check data conetent type request and Create and open a file
    std::stringstream nameFile ;

    nameFile << cl.block_server[cl.nBlock].upload_file << "/Up_" << cl.get_current_time('m') << get_lastEnd_content_type(cl.client->request_data_struct->content_Type.c_str());
    std::ofstream PostFile(nameFile.str());
    std::cout << "file name "<< cl.client->request_data_struct->content_Type <<std::endl;
    // Write in to the file
    PostFile << cl.client->request_data_struct->body;
    PostFile.close();
    drop_client(cl.client);
    return 0;
}

int responseClient::deleteMethod(responseClient& cl)
{
    std::cout << "DELETE methode" << " \n URI = " << cl.client->request_data_struct->path << std::endl;
    cl.client->request_data_struct->path = cl.client->request_data_struct->path;

    cl.tmp_string = cl.root + cl.client->request_data_struct->path;

    if (remove(cl.tmp_string.c_str()))
        std::cout << "can't remove " << cl.tmp_string << std::endl;
    cl.buff << "HTTP/1.1 " << cl.statusCodes["202"] \
                << "\r\nContent-Type: "  << "text/plain"\
                << "\r\nContent-Length: " << "7" \
                << "\r\nConnection: close\r\n"\
                << "\r\nDeleted";
        cl.client->flagResponse->isReading = false;
    if (cl.sendHeader(cl.client->socket, cl.buff.str().c_str(), cl.buff.str().length()))
        return -1;
    return 0;
}