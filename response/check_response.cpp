#include "response.hpp"

int     responceClient::checkUri(std::string uri)
{
    struct stat info;

    std::cout << uri << std::endl;
    if (stat(uri.c_str(), &info) != 0)
        return 404;
    
    return 0;
}

int responceClient::getMethode(std::string methode)
{
    std::cout << methode << std::endl;
    return 0;
}

int responceClient::postMethode(std::string methode)
{
    std::cout << methode << std::endl;
    return 0;
}

int responceClient::deleteMethode(std::string methode)
{
    std::cout << methode << std::endl;
    return 0;
}

responceClient::responceClient()
{
    methodeFunction["GET"] = getMethode;
    methodeFunction["POSTE"] = postMethode;
    methodeFunction["DELETE"] = deleteMethode;
}

int responceClient::ft_response(responceClient *res_client)
{
    if (MAX_REQUEST_SIZE == res_client->client->received)
        return  400; // return status code 400
    
    methodeFunction[res_client->client->request_data_struct->method]("TEST");

    // std::cout << "8888888888888----8888888888888" << std::endl;
    // std::cout << res_client->client->request_data_struct->method << std::endl;
    // std::cout << res_client->client->request_data_struct->path << std::endl;
    // std::cout << res_client->client->request_data_struct->host << std::endl;
    // std::cout << res_client->client->request_data_struct->content_Length << std::endl;
    // std::cout << res_client->client->request_data_struct->transfer_Encoding << std::endl;
    // std::cout << res_client->client->request_data_struct->content_Type << std::endl;
    //std::cout << "---->im the res_client->client number: << " << res_client->client->socket << ">>\nhere your request -->" << data << std::endl;
    //pars_request(res_client->client);
    //sleep(30);
    char test_response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nJrifi 20 rjal";
    //send(res_client->client->socket, test_response, sizeof(test_response), 0);
    write (res_client->client->socket, (char *)test_response, sizeof(test_response));
    return 0;
}