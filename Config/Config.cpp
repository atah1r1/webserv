/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:34 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/27 17:46:04 by aes-salm         ###   ########.fr       */
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
		// std::vector<std::string>::iterator it2 = it->getServerNames().begin();
		// for (; it2 != it->getServerNames().end(); ++it2)
		// {
		// 	std::cout << "Server name: " << *it2 << std::endl;
		// }
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
			std::map<std::string, std::string> _cgis = (*itl)->_cgis;
			for (std::map<std::string, std::string>::iterator itc = _cgis.begin(); itc != _cgis.end(); ++itc)
			{
				std::cout << "CGI "
						  << "Extension: " << itc->first << " Path: " << itc->second << std::endl;
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
			// if (!(*it).getServerNames().empty() && (*it).getPort() != 0)
			// {
			//     std::vector<std::string> server_names = (*it).getServerNames();
			//     int port = (*it).getPort();
			//     ++it;
			//     for (std::vector<std::string>::iterator it; it != server_names.end(); ++it)
			//     {
			//         if ((*it) == (*it))
			//         {
			//             std::cout << "Error: Server name " << *it << " is already defined" << std::endl;
			//             exit(1);
			//         }
			//     }
			// }
			std::vector<Location *> _locations = it->getLocations();
			for (std::vector<Location *>::iterator itl = _locations.begin(); itl != _locations.end(); ++itl)
			{
				// std::cout << (*itl)->_location << std::endl;
				if ((*itl)->_location[0] != '/')
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

	if (!inFile)
	{
		std::cerr << "Unable to open file " << file_name << std::endl;
		exit(1); // call system to stop
	}

	std::stringstream strStream;
	strStream << inFile.rdbuf();	   // read the file
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