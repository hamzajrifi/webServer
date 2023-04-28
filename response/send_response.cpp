#include "response.hpp"

int responseClient::send_data()
{
    std::cout << "sending .... " << std::endl;
    // std::cout << "fd = " << client->socket << " content lenght : " << client->flagResponse->content_length << std::endl;
    
    memset(res_body, 0, BSIZE);
    client->flagResponse->file_RW.read(res_body, BSIZE);
    // std::cout << "body data = " << client->socket << " [][] " << res_body << std::endl;

    if (write(client->socket, res_body, client->flagResponse->file_RW.gcount()) < 0)
        std::cout << "write error" << std::endl;

    client->flagResponse->lenRead += client->flagResponse->file_RW.gcount();
    // if (client->flagResponse->file_RW.eof())
    if (client->flagResponse->file_RW.gcount() == 0)
    {
        client->flagResponse->isReading = false;
        client->flagResponse->file_RW.close();
        // std::cout << "file close sending.........." << std::endl;
        drop_client(client);
    }
    return 0;
}