#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include "../webServ/header.h"
#include <sys/stat.h>
#include <fstream>
#include <sstream>

#define BSIZE 1024
struct client_info;
struct config_file;

class  responceClient{
    
    public:
    client_info *client;
    client_info *next;

    char    buffer_response[BSIZE];
    char    statusCode[20];
    int     nbrstatus;
    char    res_body[BSIZE];
    int     contenet_lenght;
    std::string content_type;
    size_t  centenet_lenght;
    ///
    char    *mediaType;
    /// block server 
    std::string root;
    std::string index;
    std::vector<config_file> &block_server;
    int nBlock;

    std::map<std::string, int (*)(std::string)> methodeFunction;
    std::map<int, std::string> statusCodes;
    /// file 
    std::fstream	file_RW;
    std::stringstream buff;
    responceClient(std::vector<config_file> &blockServer);
    int         checkUri(std::string);
    static int  getMethode(std::string methode);
    static int  postMethode(std::string methode);
    static int  deleteMethode(std::string methode);
    int         ft_response(responceClient *res_client);
    int         noLocation();
    int         send_data();
    void        send_404();
};

const char *get_content_type(const char* path) ;
//// utils.cpp

#endif