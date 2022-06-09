/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:34 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/09 09:12:27 by atahiri          ###   ########.fr       */
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

Config::Config(std::string file_name)
{
    std::ifstream inFile;
    inFile.open(file_name); //open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
    std::string str = strStream.str(); //str holds the content of the file

    inFile.close();
    std::cout << str << std::endl;
    Lexer lexer(str);
    Parser parser(lexer);
    std::vector<ServerConfig> config = parser.parse();
    for (std::vector<ServerConfig>::iterator it = config.begin(); it != config.end(); ++it)
    {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ServerName: " << it->getServerName() << std::endl;
        std::cout << "IP: " << it->getServerIp() << std::endl;
        std::cout << "Port: " << it->getPort() << std::endl;
        std::cout << "Root: " << it->getRoot() << std::endl;
        std::cout << "Client buffer size: " << it->getClientBufferSize() << std::endl;
        std::cout << "Auto index: " << (it->getAutoIndex() ? "true" : "false") << std::endl;
        std::cout << "Allow methods: " << it->getAllowMethods().front() << std::endl;
        std::cout << "Error pages: " << it->getErrorPages().front() << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}

Config::~Config()
{
}