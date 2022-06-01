/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/01 16:02:29 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"

int main()
{
    Config *config = new Config();
    std::cout << "Root " << config->getRoot() << std::endl;
    std::cout << "Port " << config->getPort() << std::endl;
    std::cout << "ServerName: " << config->getServerName() << std::endl;
    std::list<std::string> test = config->getErrorPages();
    for (std::list<std::string>::iterator t = test.begin(); t != test.end(); ++t)
    {
        std::cout << "Error pages: " << *t << std::endl;
    }
    return 0;
}