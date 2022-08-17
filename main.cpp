/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/17 22:03:58 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"
#include "Socket/Server.hpp"
#include "Request/Request.hpp"
#include <signal.h>

// void handle_signal_action(int sig_number)
// {
//     if (sig_number == SIGINT) {
//         std::cout << " { SIGINT was catched! }" << std::endl;
//         exit(EXIT_SUCCESS);
//     } else if (sig_number == SIGPIPE) {
//         std::cout << " { SIGPIPE was catched! }" << std::endl;
//     }
// }

// int setup_signals()
// {
//     struct sigaction sa;
//     sa.sa_handler = handle_signal_action;
//     if (sigaction(SIGINT, &sa, 0) != 0)
//     {
//         perror("sigaction()");
//         return -1;
//     }
//     if (sigaction(SIGPIPE, &sa, 0) != 0)
//     {
//         perror("sigaction()");
//         return -1;
//     }
//     return 0;
// }

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return (0);
    }

    Config *config = new Config();
    std::vector<ServerConfig> servers = config->getServers(argv[1]);
    Server server;
    server.setServConf(servers);
    //setup_signals();
    signal(SIGPIPE, SIG_IGN);

    try {
        server.start();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

