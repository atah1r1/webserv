/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/16 01:58:40 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"
#include "Socket/Server2.hpp"
#include "Request/Request.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return (0);
    }

    Config *config = new Config();
    std::vector<ServerConfig> servers = config->getServers(argv[1]);
    Server2 server;
    server.setServConf(servers);

    try {
        server.start();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

