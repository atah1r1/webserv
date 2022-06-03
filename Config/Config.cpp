/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:34 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/03 20:14:30 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <fstream>


Config::Config(/* args */)
{
}

const std::string WHITESPACE = " \r\t\f\v";

std::string ltrim(const std::string &s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
	return rtrim(ltrim(s));
}

Config::Config(std::string file_name)
{
    // std::cout << file_name << std::endl;
    std::ifstream input;
    input.open(file_name);
    // bool server_opened = false;
    if (input.is_open())
    {
        char str[2048];
        while (input.getline(str, 2048))
        {
            std::string line(str);
            line += "\n";
            line = trim(line);
            std::string key = "server";
            std::string token;
            for(size_t i = 0; i < line.length(); i++)
            {
                if (line[i] != ' ')
                {
                    token += line[i];
                }
                if (token == "server")
                {
                    std::cout << "i found server";
                }
            }
        }
    }
    input.close();
}

Config::~Config()
{
}