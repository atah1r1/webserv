/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 15:40:19 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/19 16:44:14 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(std::vector<ServerConfig> servers)
{
    for(std::vector<ServerConfig>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        this->_init((*it).getServerIp(), (*it).getPort());
        this->_socket();
        this->_bind();
        this->_listen();
        this->_accept();
    }
    this->_recv();
    this->_send(this->hello);
    this->_close();
}

void Socket::_init(std::string host, int port)
{
    this->opt = 1;
    this->port = port;
    this->ip = host;
    this->hello = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
}

// AF_INET ipv4
// SOCK_STREAM tcp
// 0 indicates that the caller does not want to specify the protocol and will leave it up to the service provider.

// set option for socket level SOL_SOCKET
// SO_REUSEADDR allows the socket to be bound to the address even if it is already in use.

void Socket::_socket()
{
    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    servers_fds.push_back(this->server_fd);
    // if (fcntl(this->server_fd, F_SETFL, O_NONBLOCK) < 0)
    // {
    //     std::cerr << "non_blocking error" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &this->opt, sizeof(int)))
    {
        std::cout << "setsockopt error" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::_bind()
{
    memset(&this->address, 0, sizeof(this->address));
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = inet_addr(this->ip.c_str());
    this->address.sin_port = htons(this->port);

    // Forcefully attaching socket to the port 8080
    if (bind(this->server_fd, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
    {
        std::cout << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// 128 backlogs

void Socket::_listen()
{
    if (listen(this->server_fd, 128) < 0)
    {
        std::cout << "listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::_accept()
{
    if ((new_socket = accept(this->server_fd, (struct sockaddr *)&this->address, (socklen_t *)&(this->address))) < 0)
    {
        std::cout << "accept failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::_send(std::string msg)
{
    send(this->new_socket, msg.c_str(), strlen(msg.c_str()), 0);
}

void Socket::_recv()
{
    memset(this->buffer, 0, 1024);
    this->valread = read(this->new_socket, this->buffer, 1024);
    std::cout << "How much line i read: " << valread << std::endl;
    std::cout << this->buffer << std::endl;
}

void Socket::_close()
{
    close(this->new_socket);
}

// Socket::Socket(Socket const &rhs)
// {
//     *this = rhs;
// }

// Socket &Socket::operator=(Socket const &rhs)
// {
//     (void)rhs;
//     return (*this);
// }

Socket::~Socket()
{
}