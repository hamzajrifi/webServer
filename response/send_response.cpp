#include "response.hpp"

int responceClient::send_data()
{
    std::string buff = "HTTP/1.1 " + statusCodes[nbrstatus] \
                        + "\r\nContent-Length: " + "10" \
                        + "\r\nContent-Type"  + content_type \
                        + "\r\n\r\n"  + res_body;

    write(client->socket, buff.c_str(), buff.length());
    return 0;
}
