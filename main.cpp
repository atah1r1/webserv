/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/26 10:53:34 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"
#include "Socket/Server.hpp"
#include "Request/Request.hpp"
#include <signal.h>

int main(int argc, char *argv[])
{
    Config *config = new Config();
    Server server;
    std::vector<ServerConfig> serverConfigs;
    if (argc < 2) {
        std::cout << "Handling sample config..." << std::endl;
        serverConfigs = config->getServers("./config.conf.sample");
        server.setServConf(serverConfigs);
    } else {    
        serverConfigs = config->getServers(argv[1]);
        server.setServConf(serverConfigs);
    }
    signal(SIGPIPE, SIG_IGN);
    try {
        std::cout << C_BLUE "✅ Webserv Server Started ..." C_RESET << std::endl;
        server.start();
    } catch (std::exception &e) {
        server.cleanup();
        std::cerr << e.what() << std::endl;
    }

    delete config;

    return 0;
}

