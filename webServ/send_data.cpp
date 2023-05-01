#include "header.h"

void just_parce_boundary(client_info *client)
{
    std::string boundary;
    if (client->request_data_struct->content_Type.compare(0, 20, "multipart/form-data;") == 0)
    {
        size_t ifind = client->request_data_struct->content_Type.find("=");
        if (ifind < client->request_data_struct->content_Type.size())
            boundary = client->request_data_struct->content_Type.substr(ifind + 1);

        size_t pos = 0;
        std::cout << "=========================================" << std::endl;
        while (pos < client->request_data_struct->body.size())
        {
            pos = client->request_data_struct->body.find(boundary);
            if (pos != 2)
            {
                s_boundary tmp_boundary_pars;
                std::string junkvar = client->request_data_struct->body.substr(0, pos - 2);
                if (junkvar.compare(0, 1, "\n"))
                    junkvar = junkvar.substr(2);
                size_t evrey_pos = junkvar.find("\n");
                if (evrey_pos > junkvar.size())
                    return;
                tmp_boundary_pars.content = junkvar.substr(evrey_pos + 3);

                size_t cont_dispositoin = junkvar.substr(0, evrey_pos).find(";");
                if (cont_dispositoin > junkvar.size())
                    return;
                if (junkvar.compare(0, 21, "Content-Disposition: ") == 0)
                    tmp_boundary_pars.Content_Disposition = junkvar.substr(0, evrey_pos).substr(21, cont_dispositoin - 21);
                if (junkvar.compare(cont_dispositoin + 1, 6, " name=") == 0)
                    tmp_boundary_pars.name = junkvar.substr(0, evrey_pos).substr(cont_dispositoin + 8, junkvar.substr(0, evrey_pos).size() - (cont_dispositoin + 10));
                client->request_data_struct->boundary_list.push_back(tmp_boundary_pars);
            }
            client->request_data_struct->body.erase(0, pos + boundary.size());
        }
    }
}

void client_send_recv(client_info *client, ft_fdSet& dataSelect, std::vector<config_file> block_server)
{
    responseClient res_client(block_server);

    if (FD_ISSET(client->socket, &dataSelect.reads))
    {
        client->received = recv(client->socket, client->request, sizeof(client->request), 0);
        if (client->received < 1)
        {
            std::cout << "i don't know the reason, but the client is disconnect. "<< std::endl;
            drop_client(client);
        }
        else if (!client->flagResponse->isReading)
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
                else if (!client->request_data_struct->content_Length.empty())//here just for content-length
                {   
                    if (client->request_data_struct->body.size() == (size_t)atoi(client->request_data_struct->content_Length.c_str()))
                        client->indice_end_body = 1;
                }
                else
                    client->request_data_struct->nbrStatus = client->request_data_struct->transfer_Encoding.empty() ? "501" : "400";
                std::cout << "here here inside req: "<< client->socket << std::endl;
                std::cout << client->data.size() << "=" << client->request_data_struct->content_Length << "=" << client->request_data_struct->body.size() << std::endl;
            }
            if (client->indice_end_body == 1)
            {
                res_client.client = client;
		    	res_client.ft_response();
            }
        }
    }
    else if (client->flagResponse->isReading && FD_ISSET(client->socket, &dataSelect.write))
    {
        res_client.client = client;
		res_client.ft_response();
    }
}
