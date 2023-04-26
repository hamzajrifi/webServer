#include "response.hpp"

int	responseClient::noLocation()
{
    std::cout << "----------- No location ---------" << std::endl;
    if (block_server[nBlock].index[0] == '/')
        index = block_server[nBlock].list_of_location[nbrLocation].index.substr(1);
    return 0;
}

int responseClient::check_if_location_matched()
{
    size_t lenPathLocation = block_server[nBlock].list_of_location[0].path.length();
    int i = 0;
    std::string rootLocation;
    nbrLocation = 0;
    for(size_t nLocation = 0; nLocation < block_server[nBlock].list_of_location.size(); nLocation++)
    {
        // std::cout << "nblock =  " << nLocation << " nlocation " << block_server[nBlock].list_of_location[nLocation].path \
        // << "path " <<  client->request_data_struct->path << std::endl;
        if (client->request_data_struct->path.find(block_server[nBlock].list_of_location[nLocation].path) != std::string::npos \
            && (client->request_data_struct->path[block_server[nBlock].list_of_location[nLocation].path.length()] == '\0' || \
            client->request_data_struct->path[block_server[nBlock].list_of_location[nLocation].path.length()] == '/'))
        {
            std::cout << "------------------------- location is matched -------------------------\n" << std::endl;
            client->flagResponse->isLocation = true;
            if (!i++ || (block_server[nBlock].list_of_location.size() > 1 && nLocation > 0 
            && lenPathLocation <= block_server[nBlock].list_of_location[nLocation].path.length()))
            {
                nbrLocation = nLocation;
                if (!block_server[nBlock].list_of_location[nLocation].allow_method.empty() && check_method())
                    return send_error_status("405");
                if (!block_server[nBlock].list_of_location[nLocation].autoindex.find("on"))
                    client->flagResponse->ifautoIndex = true;
                if (!block_server[nBlock].list_of_location[nLocation].returno.empty())
                {
                    index = block_server[nBlock].list_of_location[nLocation].returno;
                    return send_error_status("301");
                }
                    // std::cout << "root " << block_server[nBlock].list_of_location[nLocation].returno << std::endl;
                if (block_server[nBlock].list_of_location[nLocation].index[0] == '/')
                    index = block_server[nBlock].list_of_location[nLocation].index.substr(1);
                else
                    index = block_server[nBlock].list_of_location[nLocation].index;
                rootLocation = client->request_data_struct->path.substr(block_server[nBlock].list_of_location[nLocation].path.length()\
                            , client->request_data_struct->path.length());
            }
        }
    }
    if (i)
        client->request_data_struct->path = rootLocation.length() == 0 ? "/" : rootLocation ;
    return 0;
}