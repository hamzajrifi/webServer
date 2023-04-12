#include "header.h"
#include <iostream>
#include <iomanip>
#include <fstream>

void print_block_server(std::vector<config_file> block_server)
{
    size_t block_size = (block_server).size();
    if ((block_server).size() == 1)
        block_size = 2;
    for(size_t i = 0; i < block_size - 1; i++)
    {
        std::cout << block_server[i].client_max_body_size << std::endl;
        std::cout << block_server[i].error_log << std::endl;
        std::cout << block_server[i].listen << std::endl;
        std::cout << block_server[i].root << std::endl;
        std::cout << block_server[i].error_pages << std::endl;
        std::cout << block_server[i].server_name << std::endl;
        std::cout << "*server : " << block_server[i].server << std::endl;
        std::cout << "*port : " << block_server[i].port << std::endl;
        std::cout << "--------------" << std::endl;
        for(size_t l = 0; l < block_server[i].list_of_location.size(); l++)
        {
            std::cout << "- - - " << l << std::endl;
            std::cout << block_server[i].list_of_location[l].allow_method << std::endl;
            std::cout << block_server[i].list_of_location[l].autoindex << std::endl;
            std::cout << block_server[i].list_of_location[l].index << std::endl;
            std::cout << block_server[i].list_of_location[l].returno << std::endl;
            std::cout << block_server[i].list_of_location[l].path << std::endl;
        }
            std::cout << "---------------" << std::endl;
        if (block_server[i].listen.empty())
            std::cout << "hi roma listen is empty" << std::endl;
        std::cout << "________________________________________________________________" << std::endl;
    }
}

void location_pars(std::vector<config_file> *block_server, std::vector<std::string> temp_split_data, size_t ifind, size_t j, size_t i)
{
    location_struct tmp_node;
    while (temp_split_data[j].compare(0, 9, "]") != 0 && j < temp_split_data.size() - 1)
    {
        ifind = temp_split_data[j].find(" ");
        if (ifind < temp_split_data[j].size())
        {
            if (temp_split_data[j].compare(0, 13, "allow_method ") == 0)
                tmp_node.allow_method = temp_split_data[j].substr(ifind + 1);
            else if (temp_split_data[j].compare(0, 7, "return ") == 0)
                tmp_node.returno = temp_split_data[j].substr(ifind + 1);
            else if (temp_split_data[j].compare(0, 10, "autoindex ") == 0)
                tmp_node.autoindex = temp_split_data[j].substr(ifind + 1);
            else if (temp_split_data[j].compare(0, 6, "index ") == 0)
                tmp_node.index = temp_split_data[j].substr(ifind + 1);
            else if (temp_split_data[j].compare(0, 9, "location ") == 0)
                tmp_node.path = temp_split_data[j].substr(ifind + 1);
        }
        j++;
    }
    (*block_server)[i].list_of_location.push_back(tmp_node);
}

void check_port(std::vector<config_file> *block_server, size_t ifind, size_t i)
{
    (*block_server)[i].port = (*block_server)[i].listen.substr(ifind + 1);
    if ((*block_server)[i].port.empty())
    {
        std::cout << "just error in port" << std::endl;
        exit(1);
    }
    unsigned long a = 0;
    while (a < (*block_server)[i].port.size())
    {
        if (!std::isdigit((*block_server)[i].port[a]))
        {
            std::cout << "we suport just digits in port. please inter a valid one or 'contact mr jrifi for more info'" << std::endl;
            exit(1);
        }
        a++;
    }
    if (stoi((*block_server)[i].port) <= 0 || stoi((*block_server)[i].port) > 65535)
    {
        std::cout << "the max port number is 65535. or 'contact mr jrifi can open one but just for you'" << std::endl;
        exit(1);
    }
}

void check_ip(std::vector<config_file> *block_server, size_t ifind, size_t i)
{
    std::vector<std::string> ip;
    std::string tmp_server;
    (*block_server)[i].server = (*block_server)[i].listen.substr(0, ifind);
    tmp_server = (*block_server)[i].server;
    char *roma = strtok((char *)((*block_server)[i].server.c_str()), ".");
    while (roma != nullptr)
    {
        ip.push_back(roma);
        roma = strtok(NULL, ".");
    }
    (*block_server)[i].server = tmp_server;
    if (ip.size() != 4)
    {
        std::cout << "error in configfile : ip" << std::endl;
        exit(1);
    }
    size_t j = 0;
    while (j < ip.size())
    {
        unsigned long a = 0;
        while (a < ip[j].size())
        {
            if (!std::isdigit(ip[j][a]))
            {
                std::cout << "we suport just digits in ip. please inter a valid one or 'contact mr jrifi for more info'" << std::endl;
                exit(1);
            }
            a++;
        }
        if (stoi(ip[j]) < 0 || stoi(ip[j]) > 255)
        {
            std::cout << "error in config file : ip" << std::endl;
            exit(1);
        }
        j++;
    }
}

void parce_errorPage(std::vector<config_file> *block_server, size_t i)
{
    size_t ifind = (*block_server)[i].error_pages.find(" ");
    if (ifind < (*block_server)[i].error_pages.size())
        (*block_server)[i].error_page_kv.insert(std::pair <std::string, std::string> ((*block_server)[i].error_pages.substr(0, ifind), (*block_server)[i].error_pages.substr(ifind + 1)));
    else
    {
        std::cout << "error in config file error page" << std::endl;
        exit(1);
    }
}

void block_to_variable(std::vector<config_file> *block_server)
{
    size_t j = 0;
    size_t block_size = (*block_server).size();
    std::vector<std::string> temp_split_data;
    if ((*block_server).size() == 1)
        block_size = 2;
    for(size_t i = 0; i < block_size - 1; i++)
    {
        // just i split server block with ";"
        char *roma = strtok((char *)((*block_server)[i].block.c_str()), ";");
        while (roma != nullptr)
        {
            temp_split_data.push_back(roma);
            roma = strtok(NULL, ";");
        }
        //just here i set the data to struct config_file
        for(; j < temp_split_data.size(); j++)
        {
            size_t ifind = temp_split_data[j].find(" ");//split, for after the space in every line
            if (ifind < temp_split_data[j].size())
            {
                if (temp_split_data[j].compare(0, 7, "listen ") == 0)
                {//127.0.0.1:8080
                    (*block_server)[i].listen = temp_split_data[j].substr(ifind + 1);
                    ifind = (*block_server)[i].listen.find(":");
                    if (ifind < temp_split_data[j].size())
                    {
                        check_port(block_server, ifind, i);
                        check_ip(block_server, ifind, i);
                    }
                    else
                    {
                        std::cout << "just error in config file listen" << std::endl;
                        exit(1);
                    }
                }
                else if (temp_split_data[j].compare(0, 12, "server_name ") == 0)
                    (*block_server)[i].server_name = temp_split_data[j].substr(ifind + 1);
                else if (temp_split_data[j].compare(0, 5, "root ") == 0)
                    (*block_server)[i].root = temp_split_data[j].substr(ifind + 1);
                else if (temp_split_data[j].compare(0, 12, "error_pages ") == 0)
                {
                    (*block_server)[i].error_pages = temp_split_data[j].substr(ifind + 1);
                    parce_errorPage(block_server, i);
                    //std::cout << "size:" << (*block_server)[i].error_page_kv.size() << std::endl;
                    //std::map<std::string, std::string>::iterator it = (*block_server)[i].error_page_kv.begin();
                    //while (it != (*block_server)[i].error_page_kv.end())
                    //{
                    //    std::cout << "content :" << it->first << "|" << it->second << std::endl;
                    //    ++it;
                    //}
                }
                else if (temp_split_data[j].compare(0, 10, "error_log ") == 0)
                    (*block_server)[i].error_log = temp_split_data[j].substr(ifind + 1);
                else if (temp_split_data[j].compare(0, 21, "client_max_body_size ") == 0)
                    (*block_server)[i].client_max_body_size = temp_split_data[j].substr(ifind + 1);
                else if (temp_split_data[j].compare(0, 6, "index ") == 0)
                    (*block_server)[i].index = temp_split_data[j].substr(ifind + 1);
                else if (temp_split_data[j].compare(0, 9, "location ") == 0)
                    // here just the location content
                    location_pars(block_server, temp_split_data, ifind, j, i);
            }
        }
        if ((*block_server)[i].listen.empty())
        {
            std::cout << "listen is necessary in that idoit server. 'jrifi is a good listener too.'" << std::endl;
            exit(1);
        }
    }
}

std::vector<config_file> content_to_list(std::string filecontent)
{
    int indice = 0;
    std::vector<config_file> block_server;
    char *roma = strtok((char *)(filecontent.c_str()), "}");
    while (roma != nullptr)
    {
        config_file tmp_node;
        tmp_node.block = roma;
        if (tmp_node.block.compare("\n") != 0)
            indice = 1;
        block_server.push_back(tmp_node);
        roma = strtok(NULL, "}");
    }
    if (indice == 0)
    {
        std::cout << "error in config file just \\n" << std::endl;
        exit(1);
    }
    block_to_variable(&block_server);//just i parse the block
    return block_server;
}

std::vector<config_file> pars_confile(char *configfile)
{
    //just i check is the file is open. and is not empty
    std::string filecontent;
    std::fstream cfile;
    cfile.open(configfile, std::ios::in);
    if (cfile.is_open())
    {
        //check if file is empty
        cfile.seekp(0, std::ios::end);
        size_t size = cfile.tellg();
        if( size == 0)
            std::cout << "File is empty" << std::endl;
        else//read the file
        {
            cfile.seekp(0, std::ios::beg);
            std::string line;
            while (getline(cfile, line))
                filecontent.append(line);
            std::vector<config_file> block_server = content_to_list(filecontent);//here were i parse the content of config file to list
            return block_server;
        }
    }
    else
    {
        std::cout << "config file is not open 'jrifi sbab'" << std::endl;
        exit(1);
    }
    exit(1);
}
