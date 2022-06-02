/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/02 14:05:51 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"

int main()
{
    Config *config = new Config();
    std::cout << "Root " << config->getRoot() << std::endl;
    std::cout << "Port " << config->getPort() << std::endl;
    std::cout << "ServerName: " << config->getServerName() << std::endl;
    std::list<std::string> err_pages = config->getErrorPages();
    for (std::list<std::string>::iterator t = err_pages.begin(); t != err_pages.end(); ++t)
    {
        std::cout << "Error pages: " << *t << std::endl;
    }
    std::list<std::string> allow_methods = config->getAllowMethods();
    for (std::list<std::string>::iterator t = allow_methods.begin(); t != allow_methods.end(); ++t)
    {
        std::cout << "Allow methods: " << *t << std::endl;
    }
    std::cout << "Client body buffer size: " << config->getClientBufferSize() << std::endl;
    std::cout << "Auto index: " << (config->getAutoIndex() ? "true" : "false") << std::endl;
    return 0;
}