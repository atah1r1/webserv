/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/19 16:25:00 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"
#include "Socket/Socket.hpp"

int main(int argc, char *argv[])
{
    (void)argv;
    if (argc < 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return (0);
    }
    Config *config = new Config();
    Socket *socket = new Socket(config->getServers(argv[1]));
    (void)socket;
    return 0;
}