#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include "../webServ/header.h"
#include <sys/stat.h>
#include <fstream>
#include <sstream>

#define BSIZE 9024
struct client_info;
struct config_file;

struct  Flag_respose{
    bool isReading;
    std::fstream	file_RW;
    size_t      lenRead;
    size_t      content_length;
    bool        ifautoIndex;
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
    static int  getMethod(responseClient&);
    static int  postMethod(responseClient&);
    static int  deleteMethod(responseClient&);
    static int  nbrDataUpload;
    int         ft_response();
    int         noLocation();
    int         check_if_location_matched();
    int         send_data();
    int         send_error_status(std::string nbStatus);
    int         error_301();
    int         get_default_error_page(std::string nbStatus);
    int         check_method();
};


const char *get_content_type(const char* path) ;
//// utils.cpp

#endif