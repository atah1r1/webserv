/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:17:10 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/26 19:57:19 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) : _maxFd(0) {}

Server::~Server() {}

void Server::setServConf(std::vector<ServerConfig> &servConf)
{
	_servConf = servConf;
}

void Server::setPorts(void)
{
	for (size_t i = 0; i < _servConf.size(); i++)
	{
		_ports.insert(std::make_pair(_servConf[i].getPort(), _servConf[i].getServerIp()));
	}
}

std::map<size_t, std::string> Server::getPorts() const
{
	return _ports;
}

std::vector<ServerConfig> Server::getServConf(void) const
{
	return _servConf;
}

void Server::initSets(void)
{
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
	FD_ZERO(&_tmpReadSet);
	FD_ZERO(&_tmpWriteSet);
}

bool Server::findResponse(int sockFd)
{
	return (_responses.find(sockFd) != _responses.end());
}

void Server::setupServerSockets(void)
{
	for (std::map<size_t, std::string>::iterator it = _ports.begin(); it != _ports.end(); it++)
	{
		Socket sock(true);
		sock.setPort(it->first);
		sock.setHost(it->second);
		sock.launchSock();

		FD_SET(sock.getSockFd(), &_readSet);
		if (sock.getSockFd() > _maxFd)
			_maxFd = sock.getSockFd();

		_sockets.push_back(sock);
	}
}

void Server::acceptClients(void)
{
	for (size_t i = 0; i < _sockets.size(); i++)
	{
		if (FD_ISSET(_sockets[i].getSockFd(), &_tmpReadSet))
		{
			socklen_t addr_size = sizeof(_sockets[i].getSockAddr());
			struct sockaddr_in sock_addr = _sockets[i].getSockAddr();
			int new_sock_fd = accept(_sockets[i].getSockFd(), (struct sockaddr *)&sock_addr, &addr_size);
			if (new_sock_fd < 0)
				throw std::runtime_error("accept() failed");
			Socket _client(false);
			_client.setSockFd(new_sock_fd);
			_client.setSockAddr(sock_addr);
			FD_SET(new_sock_fd, &_readSet);
			if (_client.getSockFd() > _maxFd)
				_maxFd = _client.getSockFd();
			_requests[new_sock_fd] = Request();
			_clients.push_back(_client);
			_states.push_back(ACCEPTED);
			std::cerr << C_BLUE "[" << _clients.size() - 1 << "] NEW CLIENT CONNECTED. " << C_RESET << std::endl;
		}
	}
}

void Server::readRequest(int &fd, size_t &index)
{
	char *buffer = new char[REQ_BUFFER_SIZE];
	bzero(buffer, REQ_BUFFER_SIZE);
	ssize_t xrecv = recv(fd, buffer, REQ_BUFFER_SIZE, 0);
	if (xrecv < 0)
	{
		std::cerr << C_RED "[" << index << "] RECV_ERROR: STOPPING: CLIENT DISCONNECTED: " << strerror(errno) << C_RESET << std::endl;
		_states[index] = ERROR;
	}
	else if (xrecv >= 0)
	{
		parseRequest(_requests[fd], buffer, xrecv);
		if (_requests[fd].getState() == Request::COMPLETED)
		{
			std::cout << "REQUEST COMPLETED" << std::endl;
			FD_CLR(fd, &_readSet);
			FD_CLR(fd, &_tmpReadSet);
			FD_SET(fd, &_writeSet);
			_states[index] = READ_REQUEST;
		}
	}
	delete[] buffer;
}

void Server::writeResponseHeaders(int &fd, size_t &index)
{
	if (!findResponse(fd))
	{
		Response res = ResponseHandler::handleRequests(_requests[fd], _servConf);
		_responses.insert(std::pair<int, Response>(fd, res));
	}
	Response &res = _responses[fd];
	if (!res.areHeadersSent())
	{
		std::string resStr = res.toStr();
		if (resStr.length() == 0)
		{
			throw std::runtime_error("Response is empty.");
		}
		ssize_t xsend = send(fd, resStr.c_str(), resStr.length(), 0);
		if (xsend < 0)
		{
			_states[index] = ERROR;
			std::cerr << C_RED "[" << index << "] SEND_HEADER_ERROR: CLIENT DISCONNECTED: " << strerror(errno) << C_RESET << std::endl;
		}
		else if (res.isBuffered())
		{
			_states[index] = WROTE_HEADERS;
			res.setHeadersSent(true);
		}
		else if (!res.isBuffered())
		{
			_states[index] = COMPLETED;
			res.setHeadersSent(true);
		}
	}
}

void Server::writeResponseBody(int &fd, size_t &index)
{
	if (!findResponse(fd))
	{
		_states[index] = ERROR;
		std::cerr << C_RED "[" << index << "] CACHED_RESPONSE_NOT_FOUND: (this shouldn't happen): " << strerror(errno) << C_RESET << std::endl;
	}
	char buffer[BUFFER_SIZE] = {0};
	Response &res = _responses[fd];
	size_t xread = res.getNextBuffer(buffer);

	if (xread > 0)
	{
		ssize_t xsend = send(fd, buffer, xread, 0);
		if (xsend < 0)
		{
			_states[index] = ERROR;
			std::cerr << C_RED "[" << index << "] SEND_BODY_FAILED: CLIENT DISCONNECTED: " << strerror(errno) << C_RESET << std::endl;
		}
	}
	else
	{
		_states[index] = COMPLETED;
	}
}

void Server::terminateClient(int &fd, size_t &index)
{
	FD_CLR(fd, &_readSet);
	FD_CLR(fd, &_tmpReadSet);
	FD_CLR(fd, &_writeSet);
	FD_CLR(fd, &_tmpWriteSet);
	int xclose = close(fd);
	if (xclose < 0)
		throw std::runtime_error("Error closing client socket.");
	_responses[fd].clearAll();
	_responses.erase(fd);
	_requests.erase(fd);
	_clients.erase(_clients.begin() + index);
	_states.erase(_states.begin() + index);
}

void Server::keepAlive(int &fd, size_t &index)
{
	FD_CLR(fd, &_writeSet);
	FD_CLR(fd, &_tmpWriteSet);
	FD_SET(fd, &_readSet);
	_states[index] = ACCEPTED;
	_responses[fd].clearAll();
	_responses.erase(fd);
	_requests.erase(fd);
	_requests[fd] = Request();
}

void Server::cleanup(void)
{
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
	FD_ZERO(&_tmpReadSet);
	FD_ZERO(&_tmpWriteSet);

	for (size_t i = 0; i < _clients.size(); i++)
	{
		int xclose = close(_clients[i].getSockFd());
		if (xclose < 0)
			throw std::runtime_error("Error closing client socket.");
	}

	for (size_t i = 0; i < _sockets.size(); i++)
	{
		int xclose = close(_sockets[i].getSockFd());
		if (xclose < 0)
			throw std::runtime_error("Error closing server socket.");
	}
}

void Server::listen(void)
{

	for (size_t i = 0; i < _clients.size(); i++)
	{
		int fd = _clients[i].getSockFd();

		// ======================= read from / write to client ======================= //
		if (_states[i] == ACCEPTED && FD_ISSET(fd, &_tmpReadSet))
		{
			readRequest(fd, i);
		}
		else if (_states[i] == READ_REQUEST && FD_ISSET(fd, &_tmpWriteSet))
		{
			writeResponseHeaders(fd, i);
		}
		else if (_states[i] == WROTE_HEADERS && FD_ISSET(fd, &_tmpWriteSet))
		{
			writeResponseBody(fd, i);
		}

		// ======================= close / keep client alive ======================= //
		if (_states[i] == ERROR)
		{
			try
			{
				terminateClient(fd, i);
			}
			catch (std::exception &e)
			{
				throw;
			}
		}
		else if (_states[i] == COMPLETED)
		{

			if (_requests[fd].getHeader(H_CONNECTION) != "close")
			{
				std::cerr << C_GREEN "[" << i << "] COMPLETED: KEEP-ALIVE" C_RESET << std::endl;
				keepAlive(fd, i);
			}
			else
			{
				std::cerr << C_GREEN "[" << i << "] COMPLETED: CLOSE" C_RESET << std::endl;
				try
				{
					terminateClient(fd, i);
				}
				catch (std::exception &e)
				{
					throw;
				}
			}
		}
	}
}

int Server::start(void)
{
	// initialize sets to null
	initSets();

	// get list of ip:port combinations from serverConfigs.
	setPorts();

	// create sockets and bind to ports
	setupServerSockets();

	while (true)
	{
		// copy original sets to temporary sets
		memcpy(&_tmpReadSet, &_readSet, sizeof(fd_set));
		memcpy(&_tmpWriteSet, &_writeSet, sizeof(fd_set));

		// perform selection on temporary sets
		int xselect = select(_maxFd + 1, &_tmpReadSet, &_tmpWriteSet, NULL, NULL);

		// catch select errors
		if (xselect < 0)
		{
			throw std::runtime_error(std::string("select failed: ") + strerror(errno));
		}

		// accept new connections if any / catch errors
		try
		{
			acceptClients();
		}
		catch (std::exception &e)
		{
			throw;
		}

		// read from / write to clients / catch errors
		try
		{
			listen();
		}
		catch (std::exception &e)
		{
			throw;
		}
	}
	return 0;
}
