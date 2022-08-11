/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/11 16:40:26 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"
#include "Socket/Server.hpp"
#include "Request/Request.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return (0);
    }
    Config *config = new Config();
    std::vector<ServerConfig> servers = config->getServers(argv[1]);
    Server::start(servers);
    return 0;
}