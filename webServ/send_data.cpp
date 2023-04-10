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
             responceClient res_client(block_server);
            res_client.client = client;
            /// check data client and send response
            res_client.ft_response(&res_client);
        }
    }
}