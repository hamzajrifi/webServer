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
    newclient->indice_header = 1;
    newclient->indice_end_body = 0;
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

fd_set wait_on_clients(std::vector<int> server_socket, ft_fdSet&  dataSelect)
{
    
    FD_ZERO(&dataSelect.reads);
    unsigned long i = 0;
    while (i < server_socket.size())
    {
        FD_SET(server_socket[i], &dataSelect.reads);
        i++;
    }
    int max_socket = server_socket[i - 1];
    struct client_info *ci = clients;
    while (ci)
    {
        FD_SET(ci->socket, &dataSelect.reads);
        if (ci->socket > max_socket)
            max_socket = ci->socket;
        ci = ci->next;
    }
    //------------------------add write for mr jrifi

    FD_ZERO(&dataSelect.write);
    ci = clients;
    while (ci)
    {
        FD_SET(ci->socket, &dataSelect.write);
        if (ci->socket > max_socket)
            max_socket = ci->socket;
        ci = ci->next;
    }
    //---------------------
    if (select(max_socket + 1, &dataSelect.reads, &dataSelect.write, 0, 0) < 0)
    {
        std::cout << "select() failed." << errno << std::endl;
        exit (1);
    }
    return dataSelect.reads;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        signal(SIGPIPE, SIG_IGN);
        std::vector<config_file> block_server = pars_confile(argv[1]);// just i pars the config file
        std::cout << block_server[0].upload_file << std::endl;
        //print_block_server(block_server);
        unsigned long i = 0;
        std::vector<int> server;
        size_t block_size = (block_server).size();
        if ((block_server).size() == 1)
            block_size = 2;
        while (i < block_size - 1)//just a create a socket for every block in config file
        {
            server.push_back(create_socket(block_server[i].server.c_str(), block_server[i].port.c_str()));
            i++;
        }
        while (1)
        {
            fd_set reads;
            ft_fdSet dataSelect;
            reads = wait_on_clients(server, dataSelect);
            i = 0;
            while (i < block_size - 1)//just a chkeck for new connection for every lisenner
            {
                if (FD_ISSET(server[i], &reads))//new connection has been detected
                {
                    struct client_info *client = get_client(-1);
                    client->flagResponse = new Flag_respose;
                    client->flagResponse->isReading = false;
                    client->flagResponse->lenRead = 0;
					client->flagResponse->ifautoIndex = true;
        			client->flagResponse->isLocation = false;

                    client->socket = accept(server[i], nullptr, nullptr);

                    if (!(client->socket >= 0))
                    {
                        std::cout << "accept() failed. " << errno << std::endl;
                        return 1;
                    }
                    std::cout << "New connection from localhost>>>> ." << server[i] << std::endl;
                }
                i++;
            }
            //client sending data
            struct client_info *client = clients;
            while (client)
            {
                struct client_info *next = client->next;
                client_send_recv(client, dataSelect, block_server);
                client = next;
            }
        }
        std::cout << "\nClosing socket..." << std::endl;
        //close(server);
        while (i < block_size - 1)
        {
            close(server[i]);
            i++;
        }
    }
    else
        std::cout << "please enter a config file or say 'Jrifi 20 rjal'" << std::endl;
}
