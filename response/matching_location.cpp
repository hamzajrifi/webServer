#include "response.hpp"

int	responseClient::noLocation()
{
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

    found  = 0;
    while((found = client->request_data_struct->path.find("//")) != std::string::npos)
        client->request_data_struct->path =  client->request_data_struct->path.erase(found, 1);

    for(size_t nLocation = 0; nLocation < block_server[nBlock].list_of_location.size(); nLocation++)
    {
        if (client->request_data_struct->path.find(block_server[nBlock].list_of_location[nLocation].path) != std::string::npos \
            && (client->request_data_struct->path[block_server[nBlock].list_of_location[nLocation].path.length()] == '\0' || \
            client->request_data_struct->path[block_server[nBlock].list_of_location[nLocation].path.length()] == '/'))
        {
            client->flagResponse->isLocation = true;
            if (!i++ || (block_server[nBlock].list_of_location.size() > 1 && nLocation > 0 
            && lenPathLocation <= block_server[nBlock].list_of_location[nLocation].path.length()))
            {
                is_method_allowed = false;
                is_redi = false;
                nbrLocation = nLocation;
                uplaodFile = block_server[nBlock].list_of_location[nLocation].upload_file;

                locationMatched = block_server[nBlock].list_of_location[nLocation].path;
                if (!block_server[nBlock].list_of_location[nLocation].allow_method.empty() && check_method())
                {
                    is_method_allowed = false;
                    continue;
                }
                if (!block_server[nBlock].list_of_location[nLocation].autoindex.find("on"))
                    client->flagResponse->ifautoIndex = true;
                if (!block_server[nBlock].list_of_location[nLocation].returno.empty())
                {
                    index = block_server[nBlock].list_of_location[nLocation].returno;
                    is_redi = true;
                    continue;
                }
                if (block_server[nBlock].list_of_location[nLocation].index[0] == '/')
                    index = block_server[nBlock].list_of_location[nLocation].index.substr(1);
                else
                    index = block_server[nBlock].list_of_location[nLocation].index;
                rootLocation = client->request_data_struct->path.substr(block_server[nBlock].list_of_location[nLocation].path.length()\
                            , client->request_data_struct->path.length());
            }
        }
    }
    if (is_method_allowed)
        return send_error_status("405");
    if (i)
        client->request_data_struct->path = rootLocation.length() == 0 ? "/" : rootLocation ;
    if (is_redi)
    {
        if (!block_server[nBlock].list_of_location[nbrLocation].upload_redi.empty())
        {
            root_directory_if_existe();
            return methodeFunction[client->request_data_struct->method](*this);
        }
        else
            return send_error_status("301");
    }
    return 0;
}