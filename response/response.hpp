#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include "../webServ/header.h"
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#define BSIZE 9024
struct client_info;
struct config_file;

struct  Flag_respose{
    std::stringstream tmp_file;
    bool        isReading;
    std::fstream	file_RW;
    size_t      lenRead;
    size_t      content_length;
    bool        ifautoIndex;
    std::string allowedMethod;
    bool        isLocation;
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
    size_t nbrLocation;
    bool    noServerMatched;

    std::map<std::string, int (*)(responseClient&)> methodeFunction;
    std::map<std::string, std::string> statusCodes;
    /// time  
    time_t rawtime;
    /// file 
    std::stringstream buff; 
    std::stringstream buff2; 
    responseClient(std::vector<config_file> &blockServer);
    int         checkUri(std::string);
    int         root_directory_if_existe();
    size_t      list_current_directory(std::string);
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
    const char *get_content_type(const char* path);

    ///cgi
    std::string cgi_path;
    int    handle_cgi();
};


char	*ft_strjoin(const char *s1, const char *s2);
//// utils.cpp

#endif