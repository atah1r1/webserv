/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 12:24:44 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/11 19:44:51 by atahiri          ###   ########.fr       */
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

#define MAX_BUFFER_SIZE 1024

#include <vector>
#include <map>

// std::string hello = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";

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
	Server();
	~Server();

	void setPorts(std::map<size_t, std::string> &ports);
	void setServConf(std::vector<ServerConfig> &servConf);
	void startServerSockets();
	void addToSet(int fd, fd_set &set);
	void deleteFromSet(int fd, fd_set &set);

	void fillSockSet();
	void performSelect();

	void acceptNewClient(Socket *sock);
	void handleClient(Socket *client);
	void sendResponse(Socket *client);

	void clean();
	void reset(int newClient);

	static int start(std::vector<ServerConfig> servers);
};

#endif