#include <iostream>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_REQUEST_SIZE 2047

struct client_request
{
    char *path;
    char *method;
};

struct client_info
{
    int socket;
    char request[MAX_REQUEST_SIZE + 1];
    int received;
    struct client_request *request_data;
    struct client_info *next;
};
static struct client_info *clients = 0;
int create_socket(const char* host, const char* port);
void pars_request(client_info *pars_request);
