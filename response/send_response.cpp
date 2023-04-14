#include "response.hpp"

int responseClient::send_data()
{
    // std::cout << "fd = " << client->socket << " content lenght : " << client->flagRespose->content_lenght << std::endl;

        client->flagRespose->file_RW.read(res_body, BSIZE);
        // std::cout << "body data = " << client->socket << " [][] " << res_body << std::endl;

        if (write(client->socket, res_body, client->flagRespose->file_RW.gcount()) < 0)
            std::cout << "write error" << std::endl;
        client->flagRespose->lenRead += client->flagRespose->file_RW.gcount();
        // if (client->flagRespose->file_RW.eof())
        if ((size_t)client->flagRespose->content_lenght <= client->flagRespose->lenRead || client->flagRespose->file_RW.gcount() == 0)
        {
            client->flagRespose->isReading = false;
            client->flagRespose->file_RW.close();
            // std::cout << "file close sending.........." << std::endl;
            drop_client(client);
        }
    
    
    return 0;
}
