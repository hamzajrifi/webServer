#include "response.hpp"

int responceClient::send_data()
{
    std::cout << "body data = " << client->socket << " [][] " << res_body << std::endl;

    write(client->socket, res_body, strlen(res_body));
    return 0;
}