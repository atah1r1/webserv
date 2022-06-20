/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/20 11:41:19 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"
#include "Socket/Socket.hpp"
#include "Socket/Utils.hpp"

void start(std::vector<ServerConfig> servers)
{
    std::vector<ServerConfig>::iterator it_b(servers.begin());
    Socket socket(servers);

    // set fd read and fd write
    fd_set SocketsRead, ReadyForRead;
    fd_set SocketsWrite, ReadyForWrite;
    set_fds(SocketsRead, SocketsWrite, socket.getServersFds());

    while (1)
    {
        std::vector<ServerConfig>::iterator server_it;
        std::cout << "waiting for connections..." << std::endl;
        std::pair<bool, std::pair<int, size_t> > search_fd;
        // select function destroys the sets it is passed, so we need to copy them
        ReadyForRead = SocketsRead;
        ReadyForWrite = SocketsWrite;
        if (select(FD_SETSIZE, &ReadyForRead,&ReadyForWrite, NULL, NULL) < 0)
        {
            std::cerr << "In select" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ReadyForRead))
            {
                if ((search_fd = find_fd(i, socket.getServersFds())).first)
                {
                    // this is a new connection
                    std::cout << "IF CONDITION" << std::endl;
                    int client_socket = socket.acceptNewConnection(search_fd.second);
                    server_it = it_b + (search_fd.second).second;
                    FD_SET(client_socket, &SocketsRead);
                }
                else
                {
                    if (socket.handleConnection(*server_it, i) == true)
                        FD_CLR(i, &SocketsRead);
                    std::cout << "ELSE" << std::endl;
                }
                // else
                // {
                //     if (!server.isRequestExist(i))
                //         server.addNewRequest(i);
                //     if (server.handleConnection(*server_it, i) == true)
                //         FD_CLR(i, &SocketsRead);
                // }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return (0);
    }
    Config *config = new Config();
    std::vector<ServerConfig> servers = config->getServers(argv[1]);
    start(servers);
    return 0;
}