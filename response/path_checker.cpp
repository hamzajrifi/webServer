#include "response.hpp"

int responseClient::checkUri(std::string _uri)
{
    // std::cout << "********** =>>> path = [" << _uri << "] <<<= **********\n" << std::endl;
    //check path if cgi uri
    uri = _uri;
    if (client->flagResponse->isLocation && !block_server[nBlock].list_of_location[nbrLocation].cgi_path.empty() 
                    && _uri.find("?") != std::string::npos)
                   _uri = pars_cgi_uri();
    if (stat(_uri.c_str(), &info) != 0)
    {
        std::cout << "path not valid" << std::endl;   
       return send_error_status("404");
    }
    else
    {
        std::cout << "path valid !" << std::endl;
        client->flagResponse->file_RW.open(_uri);
        if (!client->flagResponse->file_RW && client->request_data_struct->method != "POST")
        {
            std::cout << " _uri not file !  " <<  _uri  << std::endl;
            if (client->flagResponse->ifautoIndex)
            {
                std::cout << "inside uri " << std::endl;
                uri = _uri;
                return 0;
            }
            else if (index.empty())
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
                std::cout << " file not valid !  " <<  _uri + index << std::endl;
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
    char *ptr = getcwd(NULL, 256);
    std::string currentDe(ptr);
    free(ptr);
    if (root[root.length() - 1] != '/')
        root = root + "/";
    root = currentDe + "/" + root;
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
        root += "/" + tmp_uriroot[i];
    root += "/";
    if (root[0] != '/')    
        root = "/" + root;
    nbrstatus = checkUri(root + client->request_data_struct->path.substr(1));

    
    return nbrstatus;
}
