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

int main()
{
    int server = create_socket(0, "8080");
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
                int r = recv(client->socket, client->request + client->received, MAX_REQUEST_SIZE - client->received, 0);
                if (r < 1)
                {
                    std::cout << "i don't know the reason, but the client is disconnect."<< std::endl;
                    drop_client(client);
                }
                else
                {
                    //std::cout << "---->im the client number: << " << client->socket << ">>\nhere your request -->" << client->request << std::endl;
                    pars_request(client);
                    //sleep(30);
                    char test_response[] = "HTTP/1.1 200 OK\r\n\r\nJrifi 20 rjal";
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
