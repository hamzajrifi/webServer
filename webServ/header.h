#ifndef     HEADER_H
#define     HEADER_H

#include <iostream>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <iterator>

#include "../response/response.hpp"
class responseClient;
struct Flag_respose;

#define MAX_REQUEST_SIZE 9047
struct location_struct
{
    std::string path;
    std::string index;
    std::string allow_method;
    std::string returno;
    std::string autoindex;
};

struct config_file
{
    std::string block;
    std::string listen;
    std::string server;
    std::string port;
    std::string server_name;
    std::string root;
    std::string index;
    std::string error_log;
    std::string error_pages;
    std::map <std::string, std::string> error_page_kv;
    std::string client_max_body_size;
    std::string allow_method;
    std::vector<location_struct> list_of_location;
};

struct  ft_fdSet{
    fd_set reads, write;
};

struct client_request
{
    std::string path;
    std::string method;
    std::string host;
    std::string body;
    std::string content_Length;
    std::string transfer_Encoding;
    std::string content_Type;
};

struct client_info
{
    int socket;
    int indice_header;
    int indice_end_body;
    char request[MAX_REQUEST_SIZE + 1];
    int received;
    struct Flag_respose *flagRespose;
    std::string data;
    struct client_request *request_data_struct;
    struct client_info *next;
};
static struct client_info *clients = 0;

int create_socket(const char* host, const char* port);
void pars_request(client_info *pars_request);
std::vector<config_file> pars_confile(char *configfile);
void pars_request_header(client_info *client);
void pars_request_body(client_info *client, std::string data);
void drop_client(struct client_info *client);
void client_send_recv(client_info *client, ft_fdSet& dataSelect, std::vector<config_file> block_server);

void print_block_server(std::vector<config_file> block_server);//just a temp fuction i will delete them later. "mat9ala9ch lina mr jrifi"

#endif