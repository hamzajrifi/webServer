#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include "../webServ/header.h"
#include <sys/stat.h>

struct client_info;
struct config_file;

class  responceClient{
    
    public:
    client_info *client;
    client_info *next;

    char    buffer_response[1024];
    char    statusCode[20];
    int     nbrstat;
    char    res_body[1024];
    int     contenet_lenght;
    char    *content_type;
    size_t  centenet_lenght;
    ///
    char    *mediaType;
    /// block server 
    std::string root;
    std::string index;
    std::vector<config_file> &block_server;

    std::map<std::string, int (*)(std::string)> methodeFunction;
    std::map<int, std::string> statusCodes;

    responceClient(std::vector<config_file> &blockServer);
    int         checkUri(std::string);
    static int  getMethode(std::string methode);
    static int  postMethode(std::string methode);
    static int  deleteMethode(std::string methode);
    int         ft_response(responceClient *res_client);
    int         noLocation();
};



#endif