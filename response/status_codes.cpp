#include "response.hpp"

int     responseClient::get_default_error_page(std::string nbStatus)
{
	buff << "HTTP/1.1 " << statusCodes[nbStatus] \
		<< "\r\nContent-Length: " << "111" \
		<< "\r\nContent-Type: text/html" \
		<< "\r\n\r\n<!DOCTYPE html><html><head><style>h1 {text-align: center;}</style></head><body><h1>Error " \
		<< nbStatus << "</h1></body></html>";
	sendHeader(client->socket, &buff.str()[0], buff.str().length());
	client->flagResponse->isReading = false;
	drop_client(client);
	return 1;
}

int	responseClient::error_301()
{
	buff 	<< "HTTP/1.1 301 Moved Permanently \r\nLocation: " << \
			client->request_data_struct->host  << "/" << index << "\r\n";
	if (sendHeader(client->socket, &buff.str()[0], buff.str().length()))
		return -1;
	drop_client(client);
	return 301;
}

int     responseClient::send_error_status(std::string nbStatus)
{
	//  error Not Implemented
	if(!nbStatus.compare("301"))
		return error_301();
	if (block_server[nBlock].error_page_kv[nbStatus].empty())
		return (get_default_error_page(nbStatus));
	client->flagResponse->file_RW.open(root + block_server[nBlock].error_page_kv[nbStatus], std::ios::in);
	if (!client->flagResponse->file_RW || block_server[nBlock].error_page_kv[nbStatus].empty())
		get_default_error_page(nbStatus);
	else
	{
		content_type = get_content_type(block_server[nBlock].error_page_kv[nbStatus].c_str());
		///get conetent lenght file
		client->flagResponse->file_RW.seekg(0, std::ios::end);
		client->flagResponse->content_length = client->flagResponse->file_RW.tellg();
		client->flagResponse->file_RW.seekg(0,std::ios::beg);
		buff    << "HTTP/1.1 " << statusCodes[nbStatus] \
				<< "\r\nContent-Length: " << client->flagResponse->content_length \
				<< "\r\nContent-Type: "  << content_type \
				<< "\r\n\r\n";

		if (sendHeader(client->socket, &buff.str()[0], buff.str().length()) < 0)
    		return -1;
	}
	nbrstatus = atoi(nbStatus.c_str());
	return nbrstatus;
}