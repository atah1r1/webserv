/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 12:21:13 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/11 16:39:16 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(bool isServ) : _isServSock(isServ), _keepAlive(false), opt(1)
{
}

Socket::~Socket()
{
}

bool Socket::operator==(const Socket &a)
{
    return (a.getSockFd() == this->getSockFd());
}

void Socket::launchSock()
{
    if (this->_isServSock)
    {
        // create socket
        if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            std::cout << "socket failed: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
        // assign  Ip, convert port to network byte order and assign local address
        this->_serv_addr.sin_family = AF_INET;
        this->_serv_addr.sin_port = htons(this->_port);
        if (this->_host == "localhost")
            this->_host = "127.0.0.1";
        this->_serv_addr.sin_addr.s_addr = inet_addr(this->_host.c_str());

        // set socket to non-blocking
		if (fcntl(this->_sockfd, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << "non_blocking error" << std::endl;
			exit(EXIT_FAILURE);
		}
		// set default socket options (reuse address)
		if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &this->opt, sizeof(int)))
		{
			std::cout << "setsockopt error" << std::endl;
			exit(EXIT_FAILURE);
		}

        // asign address to socket
        if (bind(this->_sockfd, (struct sockaddr *)&this->_serv_addr, sizeof(this->_serv_addr)) == -1)
        {
            std::cout << "bind failed: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
        // prepare the server for incoming clients requests
        if (listen(this->_sockfd, QUEUE_SIZE) == -1)
        {
            std::cout << "listen failed: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

bool Socket::isServSock() const
{
    return this->_isServSock;
}

bool Socket::keepAlive() const
{
    return this->_keepAlive;
}
void Socket::m_close() const
{
    close(this->_sockfd);
}
void Socket::setPort(int port)
{
   this->_port = port; 
}

void Socket::setHost(std::string host)
{
    this->_host = host;
}

struct sockaddr_in Socket::getSockAddr()
{
    return this->_serv_addr;
}

void Socket::setSockFd(int fd)
{
    this->_sockfd = fd;
}

void Socket::setSockAddr(struct sockaddr_in servAddr)
{
    this->_serv_addr = servAddr;
}

int Socket::getSockFd() const
{
    return this->_sockfd;
}

int Socket::getPort() const
{
    return this->_port;
}

void Socket::updateConnection(bool connec)
{
    this->_keepAlive = connec;
}