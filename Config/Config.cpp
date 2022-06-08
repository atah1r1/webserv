/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:34 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/08 19:48:02 by atahiri          ###   ########.fr       */
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
    for(int i = 0; i < 1; i++)
    {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ServerName: " << config[i].getServerName() << std::endl;
        std::cout << "Port: " << config[i].getPort() << std::endl;
        std::cout << "Root: " << config[i].getRoot() << std::endl;
        std::cout << "Client buffer size: " << config[i].getClientBufferSize() << std::endl;
        std::cout << "Auto index: " << (config[i].getAutoIndex() ? "true" : "false") << std::endl;
        std::cout << "Allow methods: " << config[i].getAllowMethods().front() << std::endl;
        std::cout << "Error pages: " << config[i].getErrorPages().front() << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}

Config::~Config()
{
}