#include "header.h"

void client_send_recv(client_info *client, fd_set reads, std::vector<config_file> block_server)
{
    (void)block_server;//hi bach takhdam biha asi jrifi
    if (FD_ISSET(client->socket, &reads))
    {
        std::cout << "socket that should served : " << client->socket << std::endl;
        std::string data;
        int i = 1;
        while ((client->received = recv(client->socket, client->request, sizeof(client->request), 0)) > 0)
        {
            data.append(client->request, client->received);
            if (i)//just i parse the header of the request.
            {
                i = 0;
                pars_request_header(client);
                if (client->request_data_struct->method.compare("GET") == 0 || client->request_data_struct->method.compare("DELETE") == 0)
                    break;
            }
            if (client->request_data_struct->method.compare("POST") == 0)
            {
                pars_request_body(client, data);
                //just i checked for chunked data
                if (client->request_data_struct->transfer_Encoding.compare("chunked") == 0)
                {
                    size_t ifind = client->request_data_struct->body.find("\r\n0\r\n");
                    if (ifind < client->request_data_struct->body.size())
                        break;
                }
                else//here just for content-length
                    if (client->request_data_struct->body.size() == (size_t)atoi(client->request_data_struct->content_Length.c_str()))
                        break;
            }
        }//end of test
        if (client->received < 1)
        {
            std::cout << "i don't know the reason, but the client is disconnect. "<< std::endl;
            drop_client(client);
        }
        else
        {
            std::cout << "8888888888888----8888888888888" << std::endl;
            std::cout << client->request_data_struct->method << std::endl;
            std::cout << client->request_data_struct->path << std::endl;
            std::cout << client->request_data_struct->host << std::endl;
            std::cout << client->request_data_struct->content_Length << std::endl;
            std::cout << client->request_data_struct->transfer_Encoding << std::endl;
            std::cout << client->request_data_struct->content_Type << std::endl;
            std::cout << "-------\n" << client->request_data_struct->body << "----\n";
            std::cout << data << std::endl;
            //std::cout << "---->im the client number: << " << client->socket << ">>\nhere your request -->" << data << std::endl;
            //pars_request(client);
            //sleep(30);
            char test_response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nJrifi 20 rjal";
            //send(client->socket, test_response, sizeof(test_response), 0);
            write (client->socket, (char *)test_response, sizeof(test_response));
        }
    }
}