#include "header.h"

void pars_request_header(client_info *client)
{
    std::vector<std::string> temp_split_data;
    client->request_data_struct = (struct client_request*) calloc(1, sizeof(struct client_request));
    // just i split request block with "\r\n\r\n" to get every line
    std::string request = client->request;
    size_t ifind = request.find("\r\n\r\n");
    std::string header = request.substr(0, ifind);
    //just here i parse the GET and DELETE, POST method
    char *roma = strtok((char *)header.c_str(), "\r\n");
    while (roma != nullptr)
    {
        temp_split_data.push_back(roma);
        roma = strtok(NULL, "\r\n");
    }
    //if (temp_split_data[0].compare(0, 5, "GET /") == 0 || temp_split_data[0].compare(0, 8, "DELETE /") == 0 || temp_split_data[0].compare(0, 6, "POST /") == 0)
    //{
        client->request_data_struct->method = strtok(client->request, " ");
        client->request_data_struct->path = strtok(NULL, " ");
        for(size_t j = 0; j < temp_split_data.size(); j++)
        {
            size_t ifind = temp_split_data[j].find(" ");// split, for after the space in every line 
            if (temp_split_data[j].compare(0, 6, "Host: ") == 0)
                client->request_data_struct->host = temp_split_data[j].substr(ifind + 1);
            else if (temp_split_data[j].compare(0, 16, "Content-Length: ") == 0)
                client->request_data_struct->content_Length = temp_split_data[j].substr(ifind + 1);
            else if (temp_split_data[j].compare(0, 19, "Transfer-Encoding: ") == 0)
                client->request_data_struct->transfer_Encoding = temp_split_data[j].substr(ifind + 1);
            else if (temp_split_data[j].compare(0, 14, "Content-Type: ") == 0)
                client->request_data_struct->content_Type = temp_split_data[j].substr(ifind + 1);
        }
    //}
}

void pars_request_body(client_info *client, std::string data)
{
    //Content-Type: multipart/form-data; boundary=----WebKitFormBoundar
    std::vector<std::string> temp_split_data;
    // just i split request block with "\r\n" to get every line
    std::string request = data;
    size_t ifind = request.find("\r\n\r\n");
    std::string body = request.substr(ifind + 4);

    client->request_data_struct->body = body;
}

void pars_request(client_info *client)
{
    std::vector<std::string> temp_split_data;
    client->request_data_struct = (struct client_request*) calloc(1, sizeof(struct client_request));
    // just i split request block with "\r\n" to get every line
    std::string request = client->request;
    size_t ifind = request.find("\r\n\r\n");
    std::string header = request.substr(0, ifind);
    std::string body = request.substr(ifind + 4);
    //just here i parse the GET and DELETE, POST method
    char *roma = strtok((char *)header.c_str(), "\r\n");
    while (roma != nullptr)
    {
        temp_split_data.push_back(roma);
        roma = strtok(NULL, "\r\n");
    }
    if (temp_split_data[0].compare(0, 5, "GET /") == 0 || temp_split_data[0].compare(0, 8, "DELETE /") == 0 || temp_split_data[0].compare(0, 6, "POST /") == 0)
    {
        client->request_data_struct->method = strtok(client->request, " ");
        client->request_data_struct->path = strtok(NULL, " ");
        if (client->request_data_struct->path.size() > 2024)
        {
            client->request_data_struct->nbrStatus = "414";
            return;
        }

        for(size_t j = 0; j < temp_split_data.size(); j++)
        {
            std::cout << "--# " << temp_split_data[j] << std::endl;
            size_t ifind = temp_split_data[j].find(" ");//split, for after the space in every line
            if (temp_split_data[j].compare(0, 6, "Host: ") == 0)
                client->request_data_struct->host = temp_split_data[j].substr(ifind + 1);
        }
    }
    if (temp_split_data[0].compare(0, 6, "POST /") == 0)//here i handle the body of post method
    {
        client->request_data_struct->body = body;
        std::cout << "body" << body << std::endl;
    }
    else
        std::cout << "we are not support this method yet. contact mr jrifi for more info" << std::endl;
}
