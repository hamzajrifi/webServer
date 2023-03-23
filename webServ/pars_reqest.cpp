#include "header.h"

void pars_request(client_info *client)
{
    std::cout << "from a pars_request() : " << client->request << std::endl;
}
