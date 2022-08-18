/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/18 18:59:35 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"
#include "Socket/Server.hpp"
#include "Request/Request.hpp"
#include <signal.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return (0);
    }

    Config *config = new Config();
    std::vector<ServerConfig> serverConfigs = config->getServers(argv[1]);
    Server server;
    server.setServConf(serverConfigs);
    signal(SIGPIPE, SIG_IGN);

    try {
        server.start();
    } catch (std::exception &e) {
        server.cleanup();
        std::cerr << e.what() << std::endl;
    }

    delete config;

    return 0;
}

