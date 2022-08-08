/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:34 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/08 02:27:30 by atahiri          ###   ########.fr       */
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
        std::map<int, std::string> _error_pages = it->getErrorPages();
        for (std::map<int, std::string>::iterator it = _error_pages.begin(); it != _error_pages.end(); ++it)
        {
            std::cout << it->first << " " << it->second << "\n";
        }
        std::vector<Location *> _locations = it->getLocations();
        // std::cout << _locations.front(). << std::endl;
        for (std::vector<Location *>::iterator itl = _locations.begin(); itl != _locations.end(); ++itl)
        {
            std::cout << "-------------LOCATIONS " << (*itl)->_location << "--------------" << std::endl;
            std::cout << "Root: " << (*itl)->_root << std::endl;
            std::cout << "Autoindex: " << (*itl)->_autoindex << std::endl;
            std::cout << "Redirection PATH: " << (*itl)->_redirection_path << std::endl;
            std::cout << "Upload store: " << (*itl)->_upload_store << std::endl;
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
            std::vector<Location *> _locations = it->getLocations();
            for (std::vector<Location *>::iterator itl = _locations.begin(); itl != _locations.end(); ++itl)
            {
                // std::cout << (*itl)->_location << std::endl;
                if ((*itl)->_location[0] == '.')
                {
                    std::cout << "Error: Location must start with /" << std::endl;
                    exit(1);
                }
            }
        }
    }
    else
    {
        for (std::vector<ServerConfig>::iterator it = config.begin(); it != config.end(); ++it)
        {
            std::vector<Location *> _locations = it->getLocations();
            for (std::vector<Location *>::iterator itl = _locations.begin(); itl != _locations.end(); ++itl)
            {
                int count = std::count((*itl)->_location.begin(), (*itl)->_location.end(), '/');
                if (count > 1)
                {
                    std::cout << "Error: Location must start with /" << std::endl;
                    exit(1);
                }
                if ((*itl)->_location[0] != '/')
                {
                    std::cout << "Error: Location must start with /" << std::endl;
                    exit(1);
                }
            }
        }
        for (std::vector<ServerConfig>::iterator it = config.begin(); it != config.end(); ++it)
        {
            std::vector<Location *> _locations = it->getLocations();
            // check duplicated locations
            for (std::vector<Location *>::iterator itl = _locations.begin(); itl != _locations.end(); ++itl)
            {
                std::vector<Location *>::iterator itl2 = itl;
                ++itl2;
                while (itl2 != _locations.end())
                {
                    if ((*itl)->_location == (*itl2)->_location)
                    {
                        std::cout << "Error: Location is duplicated" << std::endl;
                        exit(1);
                    }
                    ++itl2;
                }
            }
        }
    }
}

std::vector<ServerConfig> Config::getServers(std::string file_name)
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
    return config;
}

Config::~Config()
{
}