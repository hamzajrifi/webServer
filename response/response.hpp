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

#define BSIZE 1024
struct client_info;
struct config_file;

struct  Flag_respose{
    std::stringstream tmp_file;
    std::fstream	  file_RW;
    std::string       allowedMethod;
    size_t      content_length;
    bool        isReading;
    bool        ifautoIndex;
    bool        isLocation;
}; 

class  responseClient{
    
    public:

    char        buffer_response[BSIZE];
    char        statusCode[20];
    int         nbrstatus;
    char        res_body[BSIZE];
    size_t      centenet_lenght;
    size_t      found;
    size_t      nbrLocation;
    int         nBlock;
    bool        noServerMatched;
    size_t      nbr_session_client;
    struct stat info;
    client_info *client;
    std::string tmp_string;
    std::string locationMatched;
    std::string content_type;
    //    block server 
    std::string uri;
    std::string root;
    std::string index;
    std::vector<int> session;
    std::vector<std::string> aEnv;
    std::vector<config_file> &block_server;


    std::map<std::string, int (*)(responseClient&)> methodeFunction;
    std::map<std::string, std::string> statusCodes;

    /// file 
    std::stringstream buff; 
    std::stringstream buff2; 
    responseClient(std::vector<config_file> &blockServer);
    static int  getMethod(responseClient&);
    static int  postMethod(responseClient&);
    static int  deleteMethod(responseClient&);
    const char  *get_content_type(const char* path);
    size_t      list_current_directory(std::string);
    int         checkUri(std::string);
    int         root_directory_if_existe();
    int         ft_response();
    int         noLocation();
    int         check_if_location_matched();
    int         send_data();
    int         send_error_status(std::string nbStatus);
    int         error_301();
    int         get_default_error_page(std::string nbStatus);
    int         check_method();
    int         read_data_from_cgi();
    /// --- cgi
    std::string     cgi_path;
    std::string&    pars_cgi_uri();
    char    **sysEnv;
    char    *argv[3];
    int     cgi_fd[2];
    int     execute_cgi_file();
    int     handle_cgi();
    ~responseClient(){
    };
};

//// utils.cpp
size_t	get_current_time(char c);

#endif