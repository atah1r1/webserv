/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:17:14 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/17 21:40:09 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER2_HPP__
#define __SERVER2_HPP__

#include <map>
#include <vector>
#include <string>

#include "../Request/Request.hpp"
#include "../Request/Utils.hpp"
#include "../Response/Response.hpp"
#include "../Response/ResponseHandler.hpp"
#include "../Config/ServerConfig.hpp"
#include "Socket.hpp"
#include "State.hpp"

class Server
{
private:
    std::map<int, Request> _requests;
    std::vector<Socket> _sockets;
    std::vector<Socket> _clients;
    std::vector<State> _states;
    std::map<int, Response> _responses;
    std::map<size_t, std::string> _ports;
	std::vector<ServerConfig> _servConf;

	fd_set _readSet;
	fd_set _writeSet;
    fd_set _tmpReadSet;
    fd_set _tmpWriteSet;
	int _maxFd;
public:
    Server( void );
    ~Server();
    void setServConf(std::vector<ServerConfig> &servConf);
    void setPorts( void );
    void initSets( void );
    std::map<size_t, std::string> getPorts() const;
    std::vector<ServerConfig> getServConf() const;
    bool findResponse(int sockFd);
    void setupServerSockets( void );
    void acceptClients( void );
    void readRequest( int& fd, size_t& index );
    void writeResponseHeaders( int& fd, size_t& index );
    void writeResponseBody( int& fd, size_t& index );
    void terminateClient( int& fd, size_t& index);
    void keepAlive( int& fd, size_t& index );
    void listen( void );
    void cleanup( void );
    int start();
};

#endif