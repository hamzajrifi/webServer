#include "header.h"

int create_socket(const char* host, const char* port)
{
    std::cout << "Configuring local address..." << std::endl;
    struct addrinfo filter;
    memset(&filter, 0, sizeof(filter));
    filter.ai_family = AF_INET;
    filter.ai_socktype = SOCK_STREAM;
    filter.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    getaddrinfo(host, port, &filter, &bind_address);

    int socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    std::cout << "Creating socket..." << socket_listen << std::endl;
    if (!(socket_listen >= 0))
    {
        std::cout << "socket() failed. " << std::endl;
        exit (1);
    }
    std::cout << "Binding socket to local address..." << std::endl;
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        std::cout << "bind() function failed." << std::endl;
        exit(1);
    }
    freeaddrinfo(bind_address);
    std::cout << "listeing..." << std::endl;
    if (listen(socket_listen, 200) < 0)
    {
        std::cout << "listen() failed." << std::endl;
        exit(1);
    }
    return socket_listen;
}
