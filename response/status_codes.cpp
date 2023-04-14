#include "response.hpp"

int        responseClient::send_error_status(std::string nbStatus)
{
        content_type = get_content_type(block_server[nBlock].error_page_kv[nbStatus].c_str());
        client->flagRespose->file_RW.open(root + block_server[nBlock].error_page_kv[nbStatus], std::ios::in);
        // std::cout << "check ---*-*-*- " << block_server[nBlock].error_page_kv[nbStatus] << std::endl;
        if(!client->flagRespose->file_RW || block_server[nBlock].error_page_kv[nbStatus].empty())
        {
            std::cout  << "file  doesn't opend: "  << std::endl;
            if (client->flagRespose->file_RW)
                client->flagRespose->file_RW.close();
            buff << "HTTP/1.1 " << statusCodes[nbStatus] \
                    << "\r\nContent-Length: " << "0" \
                    << "\r\nContent-Type: "  << content_type \
                    << "\r\n\r\n";
            write(client->socket, &buff.str()[0], buff.str().length());
            client->flagRespose->isReading = false;
            drop_client(client);
        }
        else
        { 
            // std::cout  << "file opend: "  << "status code numbre :" << nbStatus << std::endl;
            ///get conetent lenght file
            client->flagRespose->file_RW.seekg(0, std::ios::end);
            client->flagRespose->content_lenght = client->flagRespose->file_RW.tellg();
            client->flagRespose->file_RW.seekg(0,std::ios::beg);
            buff    << "HTTP/1.1 " << statusCodes[nbStatus] \
                    << "\r\nContent-Length: " << client->flagRespose->content_lenght \
                    << "\r\nContent-Type: "  << content_type \
                    << "\r\n\r\n";

            write(client->socket, &buff.str()[0], buff.str().length());
            // send_data();
        }
        // std::cout << "here status code file : " << std::endl;
        nbrstatus = atoi(nbStatus.c_str());
        return nbrstatus;
}