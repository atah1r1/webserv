/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:34 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/06 18:03:47 by atahiri          ###   ########.fr       */
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
    parser.parse();
}

Config::~Config()
{
}