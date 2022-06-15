/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 15:40:19 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/15 21:27:26 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#define PORT 8080

Socket::Socket(/* args */)
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    std::string hello = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";

    // Creating socket file descriptor
    // AF_INET - IPv4, SOCK_STREAM - TCP
    // 0 indicates that the caller does not want to specify the protocol
    // and will leave it up to the service provider.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    /*  This socket option tells the kernel that even if
        this port is busy (in the TIME_WAIT state), go ahead and reuse it anyway.
        If it is busy, but with another state, you will still get an address already in use error.
        It is useful if your server has been shut down,
        and then restarted right away while sockets are still active on its port */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)))
    {
        std::cout << "setsockopt error" << std::endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        std::cout << "listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            std::cout << "accept failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        valread = read(new_socket, buffer, 1024);
        std::cout << buffer << std::endl;
        std::cout << "How much line i read: " << valread << std::endl;
        send(new_socket, hello.c_str(), strlen(hello.c_str()), 0);
    }
    // printf("Hello message sent\n");

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    // shutdown(server_fd, SHUT_RDWR);
}

Socket::~Socket()
{
}