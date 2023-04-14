#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include "../webServ/header.h"
#include <sys/stat.h>
#include <fstream>
#include <sstream>

#define BSIZE 4024
struct client_info;
struct config_file;

struct  Flag_respose{
    bool isReading;
    std::fstream	file_RW;
    size_t      lenRead;
    size_t      content_lenght;
}; 

class  responseClient{
    
    public:
    client_info *client;
    client_info *next;

    char    buffer_response[BSIZE];
    char    statusCode[20];
    int     nbrstatus;
    char    res_body[BSIZE];
    std::string content_type;
    size_t  centenet_lenght;
    ///
    char    *mediaType;
    std::string uri;
    /// block server 
    std::string root;
    std::string index;
    std::vector<config_file> &block_server;
    int nBlock;

    std::map<std::string, int (*)(responseClient&)> methodeFunction;
    std::map<std::string, std::string> statusCodes;
    /// file 
    std::stringstream buff; 
    responseClient(std::vector<config_file> &blockServer);
    int         checkUri(std::string);
    int         root_directory_if_existe();
    static int  getMethode(responseClient&);
    static int  postMethode(responseClient&);
    static int  deleteMethode(responseClient&);
    int         ft_response();
    int         noLocation();
    int         check_if_location_matched();
    int         send_data();
    int         send_error_status(std::string nbStatus);
};

const char *get_content_type(const char* path) ;
//// utils.cpp

#endif