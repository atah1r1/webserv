/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 12:24:44 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/11 13:28:49 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "../Config/ServerConfig.hpp"
#include "Socket.hpp"
#include "../Request/Request.hpp"
#include "../Response/ResponseHandler.hpp"
#include "../Response/Response.hpp"
#include "../Request/Utils.hpp"

#define MAX_BUFFER_SIZE 1024 * 20

#include <vector>
#include <map>

std::string hello = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";

class Server
{
private:
	std::vector<Socket *> _sockets;
	std::map<size_t, std::string> _ports;
	std::vector<ServerConfig> _servConf;

	fd_set _readSet;
	fd_set _writeSet;
	int _maxFd;
	std::map<int, Request> _clients;

public:
	Server() : _maxFd(-1) {}
	~Server() { this->clean(); }
	void setPorts(std::map<size_t, std::string> &ports) { this->_ports = ports; }

	void setServConf(std::vector<ServerConfig> &servConf) { this->_servConf = servConf; }

	void startServerSockets()
	{
		// create a server socket for each port
		for (std::map<size_t, std::string>::iterator it = this->_ports.begin(); it != this->_ports.end(); it++)
		{
			Socket *sock = new Socket(true);
			sock->setPort(it->first);
			sock->setHost(it->second);
			this->_sockets.push_back(sock);
		}

		for (size_t i = 0; i < this->_sockets.size(); i++)
		{
			if (this->_sockets[i]->isServSock())
				this->_sockets[i]->launchSock();
		}
	}

	void addToSet(int fd, fd_set &set)
	{
		FD_SET(fd, &set);

		// update our maxFd if the new fd is greater than madFd
		if (fd > this->_maxFd)
			this->_maxFd = fd;
	}

	void deleteFromSet(int fd, fd_set &set) { FD_CLR(fd, &set); }

	void fillSockSet()
	{
		// reset our sets
		FD_ZERO(&this->_readSet);
		FD_ZERO(&this->_writeSet);

		// add server sockets to the read set
		for (size_t i = 0; i < this->_sockets.size(); i++)
			addToSet(this->_sockets[i]->getSockFd(), this->_readSet);
	}

	void performSelect()
	{
		fd_set tmpReadSet = this->_readSet;
		fd_set tmpWriteSet = this->_writeSet;

		struct timeval timeLimit = {0, 1e3};

		// wait for events in ReadSet and WriteSet
		int result = select(this->_maxFd + 1, &tmpReadSet, &tmpWriteSet, NULL, &timeLimit);
		if (result > 0)
		{
			for (size_t i = 0; i < this->_sockets.size(); i++)
			{
				// check if a file descriptor is ready for read
				if (FD_ISSET(this->_sockets[i]->getSockFd(), &tmpReadSet))
				{
					if (this->_sockets[i]->isServSock())
						acceptNewClient(this->_sockets[i]);
					else
						handleClient(this->_sockets[i]);
				}

				// check if a file descriptor is ready for write
				if (i < _sockets.size() && FD_ISSET(this->_sockets[i]->getSockFd(), &tmpWriteSet))
				{
					sendResponse(this->_sockets[i]);
				}
			}
		}
	}

	void acceptNewClient(Socket *sock)
	{
		// accept connection on server socket and get fd for new Client
		errno = 0;
		int newClient = accept(sock->getSockFd(), 0, 0);
		Socket *client = new Socket(false);
		client->setSockFd(newClient);

		client->setSockAddr(sock->getSockAddr());
		Request req;
		this->_clients.insert(std::make_pair(newClient, req));

		this->_sockets.push_back(client);

		// add our client to read set
		addToSet(client->getSockFd(), this->_readSet);
	}

	void handleClient(Socket *client)
	{
		char buff[MAX_BUFFER_SIZE];

		// receive data from client
		int size = recv(client->getSockFd(), buff, MAX_BUFFER_SIZE, 0);

		// if an error occured or when a stream socket peer has performed a shutdown.
		if (size == -1 || size == 0)
		{
			deleteFromSet(client->getSockFd(), this->_readSet);
			deleteFromSet(client->getSockFd(), this->_writeSet);
			this->_clients.erase(client->getSockFd());
			client->m_close();
			std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), client);
			if (position != this->_sockets.end())
			{
				delete (*position);
				this->_sockets.erase(position);
			}
			return;
		}

		// send to parser and check return value if reading is complete
		if (size > 0)
		{
			std::string newStr = std::string(buff, size);
			parseRequest(this->_clients[client->getSockFd()], newStr);
			bool isComplete = this->_clients[client->getSockFd()].getState() == Request::COMPLETED;
			if (isComplete)
			{
				client->updateConnection(this->_clients[client->getSockFd()].getHeader("Connection") == "Keep-alive");
				deleteFromSet(client->getSockFd(), this->_readSet);
				addToSet(client->getSockFd(), this->_writeSet);
			}
		}
	}

	void sendResponse(Socket *client)
	{
		Response r = ResponseHandler::handleRequests(this->_clients[client->getSockFd()], this->_servConf.front());
		// send(client->getSockFd(), hello.c_str(), strlen(hello.c_str()), 0);
		std::string response = r.toString();

		if (!response.empty())
		{
			int size = send(client->getSockFd(), response.c_str(), response.size(), 0);
			char buffer[BUFFER_SIZE] = {0};
			size_t len = 0;
			if (r.isChunked())
			{
				size_t s = 0;
				while ((len = r.getNextChunk(buffer)) > 0)
				{
					send(client->getSockFd(), buffer, len, 0);
					memset(buffer, 0, BUFFER_SIZE);
					s += len;
				}
				std::cerr << "==== SIZE: " << s << std::endl;
			}
			if (size == -1)
			{
				deleteFromSet(client->getSockFd(), this->_readSet);
				deleteFromSet(client->getSockFd(), this->_writeSet);
				this->_clients.erase(client->getSockFd());
				client->m_close();
				std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), client);
				if (position != this->_sockets.end())
				{
					delete (*position);
					this->_sockets.erase(position);
				}
				return;
			}
		}
		deleteFromSet(client->getSockFd(), this->_writeSet);
		this->_clients.erase(client->getSockFd());
		client->m_close();
		std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), client);
		if (position != this->_sockets.end())
		{
			delete (*position);
			this->_sockets.erase(position);
		}
	}

	void clean()
	{
		for (size_t i = 0; i < this->_sockets.size(); i++)
		{
			this->_sockets[i]->m_close();
			delete _sockets[i];
		}
		this->_sockets.clear();
	}

	static int start(std::vector<ServerConfig> servers)
	{
		Server server;
		std::map<size_t, std::string> ports;
		for (size_t i = 0; i < servers.size(); i++)
			ports.insert(std::make_pair(servers[i].getPort(), servers[i].getServerIp()));
		server.setPorts(ports);
		server.setServConf(servers);
		server.startServerSockets();
		server.fillSockSet();
		std::cout << "WebServ: running" << std::endl;
		while (1)
		{
			signal(SIGPIPE, SIG_IGN);
			server.performSelect();
		}
		server.clean();
		return 0;
	}
};

#endif