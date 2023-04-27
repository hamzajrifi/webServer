#include "response.hpp"

int responseClient::checkUri(std::string _uri)
{
    struct stat info;

    // std::cout << "********** =>>> path = [" << _uri << "] <<<= **********\n" << std::endl;
    if (stat(_uri.c_str(), &info) != 0)
    {
        // std::cout << "path not valid" << std::endl;   
       return send_error_status("404");
    }
    else
    {
        // std::cout << "path valid !" << std::endl;
        client->flagResponse->file_RW.open(_uri);
        if (!client->flagResponse->file_RW)
        {
            // std::cout << " _uri not file !  " <<  _uri  << std::endl;
            if (client->flagResponse->ifautoIndex)
            {
                // std::cout << "inside uri " << std::endl;
                uri = _uri;
                return 0;
            }
            else if (block_server[nBlock].list_of_location[nbrLocation].index.empty())
                return send_error_status("403");
            if (!client->request_data_struct->method.find("DELETE"))
                return send_error_status("409");
            if (client->flagResponse->isLocation 
                && client->request_data_struct->path.length() > 1 
                && block_server[nBlock].index[0] != '/')
                    index = "/" + index;
            client->flagResponse->file_RW.open(_uri + index);

            if (!client->flagResponse->file_RW)
            {
                // std::cout << " file not valid !  " <<  _uri + index << std::endl;
                return send_error_status("404");
            }
            uri = _uri + index;
        }
        else
            uri = _uri;
    }
    return 0;
}

int responseClient::root_directory_if_existe()
{
    std::vector<std::string> tmp_uriroot;
    char tmp[256];
    std::string currentDe(getcwd(tmp, 256));
    if (root[root.length() - 1] != '/')
        root = root + "/";
    root = currentDe + "/" + root;
    std::cout << "root" << root << std::endl;
    char *roma = strtok((char *)root.c_str(), "/");
    while (roma != nullptr)
        {
            tmp_uriroot.push_back(roma);
            roma = strtok(NULL, "/");
        }
    for (std::vector<std::string>::iterator it =  tmp_uriroot.begin(); it != tmp_uriroot.end();)
    {
        if (it->compare("..") == 0 || it->compare(".") == 0)
        {
            if (it != tmp_uriroot.begin() && it->compare("..") == 0)
                it = tmp_uriroot.erase(it - 1);
            it = tmp_uriroot.erase(it);
        }
        else
           ++it;
    }
    root.clear();
    for (size_t i = 0; i < tmp_uriroot.size(); i++)
        {
            root += "/" + tmp_uriroot[i];
            std::cout << " arg = "<< tmp_uriroot[i] << std::endl;
        }
    std::cout << root << std::endl;
    root += "/";
    if (root[0] != '/')    
        root = "/" + root;
        //std::cout << "request path "<< client->request_data_struct->path << "\n "<< std::endl;
    nbrstatus = checkUri(root + client->request_data_struct->path.substr(1, client->request_data_struct->path.length()));
    return nbrstatus;
}