#include "response.hpp"

int responseClient::send_data()
{
        std::cout << "fd = " << client->socket << " content lenght : " << client->flagRespose->content_lenght << std::endl;
    while (1)
    {
        client->flagRespose->file_RW.read(res_body, BSIZE);
        // std::cout << "body data = " << client->socket << " [][] " << res_body << std::endl;
        if (write(client->socket, res_body, strlen(res_body)) < 0)
            std::cout << "write error" << std::endl;
        client->flagRespose->lenRead += strlen(res_body);
        // if (client->flagRespose->file_RW.eof())
        if ((size_t)client->flagRespose->content_lenght <= client->flagRespose->lenRead)
        {
            client->flagRespose->isReading = false;
            client->flagRespose->file_RW.close();
            break;
        } 
    }
    std::cout << "sending.........." << std::endl;
    
    return 0;
}
