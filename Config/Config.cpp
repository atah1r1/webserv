/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:34 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/14 23:09:05 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <fstream>
#include <sstream>

#include "Lexer.hpp"
#include "Parser.hpp"

Config::Config(/* args */)
{
}

void PrintData(std::vector<ServerConfig> config)
{
    for (std::vector<ServerConfig>::iterator it = config.begin(); it != config.end(); ++it)
    {
        std::cout << "-------------SERVER--------------" << std::endl;
        std::cout << "ServerName: " << it->getServerName() << std::endl;
        std::cout << "IP: " << it->getServerIp() << std::endl;
        std::cout << "Port: " << it->getPort() << std::endl;
        std::cout << "Client buffer size: " << it->getClientBufferSize() << std::endl;
        std::vector<std::string> _error_pages = it->getErrorPages();
        for (std::vector<std::string>::iterator ite = _error_pages.begin(); ite != _error_pages.end(); ++ite)
        {
            std::cout << "Error page"
                      << "[" << ite - _error_pages.begin() << "]: " << *ite << std::endl;
        }
        std::vector<Location *> _locations = it->getLocations();
        // std::cout << _locations.front(). << std::endl;
        for (std::vector<Location *>::iterator itl = _locations.begin(); itl != _locations.end(); ++itl)
        {
            std::cout << "-------------LOCATIONS--------------" << std::endl;
            std::cout << "Root: " << (*itl)->_root << std::endl;
            std::cout << "Autoindex: " << (*itl)->_autoindex << std::endl;
            std::cout << "Redirection PATH: " << (*itl)->_redirection_path << std::endl;
            std::vector<std::string> _allow_methods = (*itl)->_allow_methods;
            for (std::vector<std::string>::iterator itm = _allow_methods.begin(); itm != _allow_methods.end(); ++itm)
            {
                std::cout << "Allow method"
                          << "[" << itm - _allow_methods.begin() << "]: " << *itm << std::endl;
            }
            std::vector<std::string> _index = (*itl)->_index_file;
            for (std::vector<std::string>::iterator iti = _index.begin(); iti != _index.end(); ++iti)
            {
                std::cout << "Index file"
                          << "[" << iti - _index.begin() << "]: " << *iti << std::endl;
            }
            std::cout << "-------------LOCATIONS--------------" << std::endl;
        }
        std::cout << "-------------SERVER--------------" << std::endl;
    }
}

void Config::checkErrors(std::vector<ServerConfig> config)
{
    if (config.size() > 1)
    {
        for (std::vector<ServerConfig>::iterator it = config.begin(); it != config.end(); ++it)
        {
            if ((*it).getServerName() != "" && (*it).getPort() != 0)
            {
                std::string server_name = (*it).getServerName();
                int port = (*it).getPort();
                ++it;
                if (server_name == (*it).getServerName() && port == (*it).getPort())
                {
                    std::cout << "Error: Server name and port are duplicated" << std::endl;
                    exit(1);
                }
            }
        }
    }
}

Config::Config(std::string file_name)
{
    std::ifstream inFile;
    inFile.open(file_name); // open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf();       // read the file
    std::string str = strStream.str(); // str holds the content of the file

    inFile.close();
    // std::cout << str << std::endl;
    Lexer lexer(str);
    Parser parser(lexer);
    std::vector<ServerConfig> config = parser.parse();
    checkErrors(config);
    // PrintData(config);
}

Config::~Config()
{
}