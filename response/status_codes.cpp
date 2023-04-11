#include "response.hpp"

void        responceClient::send_404()
{
    nbrstatus = 400;
        content_type = get_content_type(".html");
        // content_type = get_content_type(block_server[nBlock].error_pages[400]);
        contenet_lenght = 0;
        // file_RW.open(block_server[nBlock].error_pages[400], std::ios::in);
        file_RW.open("../index.html", std::ios::in);
        if(!file_RW)
        {
            buff << "HTTP/1.1 " << statusCodes[nbrstatus] \
                    << "\r\nContent-Length: " << contenet_lenght \
                    << "\r\nContent-Type: "  << content_type \
                    << "\r\n\r\n";
            write(client->socket, &buff.str()[0], buff.str().length());
        }
        else
        {
            file_RW.seekg(0, std::ios::end);
            contenet_lenght = file_RW.tellg();
            file_RW.seekg(0,std::ios::beg);

            buff << "HTTP/1.1 " << statusCodes[nbrstatus] \
                        << "\r\nContent-Length: " << contenet_lenght \
                        << "\r\nContent-Type: "  << content_type \
                        << "\r\n\r\n";

            write(client->socket, &buff.str()[0], buff.str().length());
            while (1)
            {
                file_RW.read(res_body, BSIZE);
                send_data();
                if (file_RW.eof())
                {
                    file_RW.close();
                    break;
                }
            }
        }
}