/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:17:03 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/07 22:10:46 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Config.hpp"
#include "Socket/Socket.hpp"
#include "Socket/Utils.hpp"
#include "Request/Request.hpp"

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
        if (select(FD_SETSIZE, &ReadyForRead, &ReadyForWrite, NULL, NULL) < 0)
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
                    // std::cout << "IF CONDITION" << std::endl;
                    int client_socket = socket.acceptNewConnection(search_fd.second);
                    server_it = it_b + (search_fd.second).second;
                    // std::cout << "server_it: " << (*server_it).getPort() << std::endl;
                    FD_SET(client_socket, &SocketsRead);
                }
                else
                {
                    if (!socket.isThisRequestExist(i))
						socket.pushNewRequest(i);
                    if (socket.handleConnection(*server_it, i) == true)
                        FD_CLR(i, &SocketsRead);
                    //socket._send(i, "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>");
                        //close(i);
                    // std::cout << "ELSE" << std::endl;
                }
            }
            // if (FD_ISSET(i, &ReadyForWrite))
            // {
            //     if ((search_fd = find_fd(i, socket.getServersFds())).first)
            //     {
            //         // this is a new connection
            //         // std::cout << "IF CONDITION" << std::endl;
            //         int client_socket = socket.acceptNewConnection(search_fd.second);
            //         server_it = it_b + (search_fd.second).second;
            //         // std::cout << "server_it: " << (*server_it).getPort() << std::endl;
            //         FD_SET(client_socket, &SocketsWrite);
            //     }
            //     else
            //     {
            //         if (socket.handleConnection(*server_it, i) == true)
            //         {
            //             std::cout << "handleConnection" << std::endl;
                        // socket._send(i, "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>");
            //             FD_CLR(i, &SocketsWrite);
            //             close(i);
            //         }
            //         // std::cout << "ELSE" << std::endl;
            //     }
            // }
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