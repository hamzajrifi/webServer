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

    if (client->flagResponse->file_RW.gcount() == 0)
    {
        client->flagResponse->isReading = false;
        client->flagResponse->file_RW.close();
        unlink(client->flagResponse->tmp_file.str().c_str());
        unlink((client->flagResponse->tmp_file.str() + "post").c_str());
        drop_client(client);
    }
    return 0;
}