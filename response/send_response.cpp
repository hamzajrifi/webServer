#include "response.hpp"

int responseClient::sendHeader(int socket, const char *str, size_t length)
{
    if (write(socket, str, length) < 0)
    {
        drop_client(client);
        return -1;
    }
    return 0;
}


int responseClient::send_data()
{
    memset(res_body, 0, BSIZE);
    client->flagResponse->file_RW.read(res_body, BSIZE);
    if (sendHeader(client->socket, res_body, client->flagResponse->file_RW.gcount()) < 0)
        return -1;
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