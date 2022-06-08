/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/08 19:30:00 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"

void    printData(Config *config)
{
    std::cout << "Root " << config->getRoot() << std::endl;
    std::cout << "Port " << config->getPort() << std::endl;
    std::cout << "ServerName: " << config->getServerName() << std::endl;
    std::vector<std::string> err_pages = config->getErrorPages();
    for (std::vector<std::string>::iterator t = err_pages.begin(); t != err_pages.end(); ++t)
    {
        std::cout << "Error pages: " << *t << std::endl;
    }
    std::vector<std::string> allow_methods = config->getAllowMethods();
    for (std::vector<std::string>::iterator t = allow_methods.begin(); t != allow_methods.end(); ++t)
    {
        std::cout << "Allow methods: " << *t << std::endl;
    }
    std::cout << "Client body buffer size: " << config->getClientBufferSize() << std::endl;
    std::cout << "Auto index: " << (config->getAutoIndex() ? "true" : "false") << std::endl;

    std::vector<Location *> locations = config->getLocations();
    for (std::vector<Location *>::iterator t = locations.begin(); t != locations.end(); ++t)
    {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Location: " << (*t)->_location << std::endl;
        std::cout << "Auto index: " << (*t)->_autoindex << std::endl;
        std::cout << "Allow methods front: " << (*t)->_allow_methods.front() << std::endl;
        std::cout << "Client buffer size: " << (*t)->_client_buffer_size << std::endl;
        // std::cout << "Index file: " << (*t)->_index_file << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    (void)argv;
    if (argc < 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return (0);
    }
    Config *config = new Config(argv[1]);
    (void)config;
    return 0;
}