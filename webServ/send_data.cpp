#include "header.h"

void client_send_recv(client_info *client, fd_set reads, std::vector<config_file> block_server)
{
    responseClient res_client(block_server);
    if (FD_ISSET(client->socket, &reads))
    {
        client->received = recv(client->socket, client->request, sizeof(client->request), 0);
        if (client->received < 1)
        {
            std::cout << "i don't know the reason, but the client is disconnect. "<< std::endl;
            drop_client(client);
        }
        else
        {
            client->data.append(client->request, client->received);
            if (client->indice_header)//just i parse the header of the request.
            {
                client->indice_header = 0;
                pars_request_header(client);
                if (client->request_data_struct->method.compare("GET") == 0 || client->request_data_struct->method.compare("DELETE") == 0 || client->request_data_struct->method.compare("POST") != 0)
                    client->indice_end_body = 1;
            }
            if (client->request_data_struct->method.compare("POST") == 0)
            {
                pars_request_body(client, client->data);
                //just i checked for chunked data
                if (client->request_data_struct->transfer_Encoding.compare("chunked") == 0)
                {
                    size_t ifind = client->request_data_struct->body.find("\r\n0\r\n");
                    if (ifind < client->request_data_struct->body.size())
                        client->indice_end_body = 1;
                }
                else//here just for content-length
                    if (client->request_data_struct->body.size() == (size_t)atoi(client->request_data_struct->content_Length.c_str()))
                        client->indice_end_body = 1;
                std::cout << "here here incide: "<< client->socket << std::endl;
                std::cout << client->data.size() << "=" << client->request_data_struct->content_Length << "=" << client->request_data_struct->body.size() << std::endl;
            }
        if (client->indice_end_body == 1)
        {
            res_client.client = client;
			res_client.ft_response();
        }
        }
    }
}
