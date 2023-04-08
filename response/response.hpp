#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include "../webServ/header.h"
#include <sys/stat.h>

struct client_info;

struct status_code{
    std::map<int, std::string> statusCodes;
    
    status_code();
};

class  responceClient{
    
    public:
    client_info *client;
    client_info *next;
    std::string res_Send;
    std::map<std::string, int (*)(std::string)> methodeFunction;
    responceClient();
    int     checkUri(std::string);
    static int getMethode(std::string methode);
    static int postMethode(std::string methode);
    static int deleteMethode(std::string methode);
    int     ft_response(responceClient *res_client);
};



#endif