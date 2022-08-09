/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 15:40:19 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/09 00:31:42 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "../Request/Request.hpp"
#include "../Request/Utils.hpp"
#include "../Response/ResponseHandler.hpp"
#include <netinet/tcp.h>

Socket::Socket(std::vector<ServerConfig> servers) : address_len(sizeof(address))
{
	std::vector<ServerConfig>::iterator it(servers.begin());
	size_t size = servers.size();
	for (size_t i = 0; i < size; i++)
	{
		this->_init((*it).getServerIp(), (*it).getPort());
		if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::cout << "socket failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		servers_fds.push_back(this->server_fd);
		it++;
		// set socket to non-blocking
		if (fcntl(this->server_fd, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << "non_blocking error" << std::endl;
			exit(EXIT_FAILURE);
		}
		// set default socket options (reuse address)
		if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &this->opt, sizeof(int)))
		{
			std::cout << "setsockopt error" << std::endl;
			exit(EXIT_FAILURE);
		}
		this->_bind();
		this->_listen();
	}
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
}

void Socket::_bind()
{
	memset(&this->address, 0, address_len);
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = inet_addr(this->ip.c_str());
	this->address.sin_port = htons(this->port);

	vec_addresses.push_back(this->address);

	// Forcefully attaching socket to the port 8080
	if (bind(this->server_fd, (struct sockaddr *)&this->address, address_len) < 0)
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

void Socket::_send(int my_socket, const char *msg, size_t length)
{
	if (send(my_socket, msg, length, 0) < 0)
	{
		std::cerr << "send failed: ";
		std::cerr << strerror(errno) << std::endl;
		std::cerr << "retry sending..." << std::endl;
		usleep(700000);
		_send(my_socket, msg, length);
		// exit(EXIT_FAILURE);
	}
	std::cerr << "send successful" << std::endl;
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

std::vector<int> Socket::getServersFds()
{
	return this->servers_fds;
}

int Socket::acceptNewConnection(std::pair<int, size_t> pair)
{
	// pair<server, position>
	int new_socket;
	if ((new_socket = accept(pair.first, (struct sockaddr *)&this->vec_addresses[pair.second], (socklen_t *)&address_len)) < 0)
	{
		std::cerr << "In accept" << std::endl;
		exit(EXIT_FAILURE);
	}
	return new_socket;
}

bool Socket::handleConnection(ServerConfig server_setup, int new_socket)
{
	// ---------------------- Reading Request --------------------------- //
	std::cerr << "RECVING REQUEST..." << std::endl;

	Request request = receiveRequest(new_socket);

	std::cerr << "HANDLING REQUEST..." << std::endl;

	Response r = ResponseHandler::handleRequests(request, server_setup);

	std::string _res = r.toString();

	std::cerr << "SENDING HEADERS..." << std::endl;

	this->_send(new_socket, _res.c_str(), _res.length());

	char buffer[BUFFER_SIZE] = {0};
	size_t len = 0;

	std::cerr << "SENDING CUNKS..." << std::endl;

	if (r.isChunked())
	{
		size_t s = 0;
		while ((len = r.getNextChunk(buffer)) > 0)
		{
			this->_send(new_socket, buffer, len);
			bzero(buffer, BUFFER_SIZE);
			s += len;
		}
		std::cerr << "==== SIZE: " << s << std::endl;
	}
	// check if request is completed
	// if (request.getState() == Request::COMPLETED)
	// {
	// 	std::cout << "ENTERED HERE" << std::endl;
	// 	close(new_socket);
	// 	return false;
	// }

	std::cerr << "SENT EVERYTHING..." << std::endl;

	r.clearAll();
	return true;
}

bool Socket::isThisRequestExist(int fd)
{
	return this->requests.find(fd) != this->requests.end();
}

void Socket::pushNewRequest(int fd)
{
	this->requests.insert(std::pair<int, Request>(fd, Request()));
}

void Socket::removeRequest(int fd)
{
	if (this->requests.find(fd) != this->requests.end())
		this->requests.erase(fd);
}

Request Socket::receiveRequest(int fd)
{
	char buffer[1024] = {0};
	long valread = 0;
	valread = recv(fd, buffer, 1024, 0);
	(void)valread;
	// std::cout << buffer << std::endl;

	parseRequest(requests[fd], buffer);
	return requests[fd];
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