#include "header.h"

struct client_info *get_client(int s)
{
    struct client_info *tmp_list_client = clients;
    //return the client if existe
    while (tmp_list_client)
    {
        if (tmp_list_client->socket == s)
            break;
        tmp_list_client = tmp_list_client->next;
    }
    if (tmp_list_client)
        return tmp_list_client;
    //creat new one if not existe
    struct client_info *newclient = (struct client_info*) calloc(1, sizeof(struct client_info));
    if (!newclient)
    {
        std::cout << "Out of memory." << std::endl;
        exit(1);
    }
    newclient->next = clients;
    clients = newclient;
    return newclient;
}

void drop_client(struct client_info *client)
{
    //delete the client "node hahaha" from client_info list
    close(client->socket);
    struct client_info **p = &clients;
    while (*p)
    {
        if (*p == client)
        {
            *p = client->next;
            free(client);
            return;
        }
        p = &(*p)->next;
    }
    std::cout << "the client you want to drop is not found." << std::endl;
    exit(1);
}

fd_set wait_on_clients(int server_socket)
{
    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(server_socket, &reads);
    int max_socket = server_socket;
    struct client_info *ci = clients;
    while (ci)
    {
        FD_SET(ci->socket, &reads);
        if (ci->socket > max_socket)
            max_socket = ci->socket;
        ci = ci->next;
    }
    if (select(max_socket + 1, &reads, 0, 0, 0) < 0)
    {
        std::cout << "select() failed." << errno << std::endl;
        exit (1);
    }
    return reads;
}

int main(int argc, char **argv)
{
    (void)argv;
    if (argc == 2)
    {
        std::vector<config_file> block_server = pars_confile(argv[1]);// just i pars the config file
        //print_block_server(block_server);// just a function to print the content to config file
        int server = create_socket(0, block_server[0].port.c_str());
        while (1)
        {
            fd_set reads;
            reads = wait_on_clients(server);

            if (FD_ISSET(server, &reads))//new connection has been detected
            {
                struct client_info *client = get_client(-1);
                client->socket = accept(server, nullptr, nullptr);

                if (!(client->socket >= 0))
                {
                    std::cout << "accept() failed. " << errno << std::endl;
                    return 1;
                }
                std::cout << "New connection from localhost>>>> ." << server << std::endl;
            }
            // client sending data
            struct client_info *client = clients;
            while(client)
            {
                struct client_info *next = client->next;
                if (FD_ISSET(client->socket, &reads))
                {
                    std::string data;
                    int i = 1;
                    while ((client->received = recv(client->socket, client->request, sizeof(client->request), 0)) > 0)
                    {
                        data.append(client->request, client->received);
                        if (i)
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
                client = next;
            }

        }
        std::cout << "\nClosing socket..." << std::endl;
        close(server);
    }
    else
        std::cout << "please enter a config file or say 'Jrifi 20 rjal'" << std::endl;
}
