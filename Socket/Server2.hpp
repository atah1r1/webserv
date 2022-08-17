/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server2.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:17:14 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/17 16:58:24 by ehakam           ###   ########.fr       */
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

class Server2
{
private:
    std::map<int, Request> _requests;
    std::vector<Socket> _sockets;
    std::vector<Socket> _clients;
    std::vector<State> _states;
    std::map<int, Response> _responses;
    std::map<size_t, std::string> _ports;
	std::vector<ServerConfig> _servConf;
    //std::map<int, State> _states;
    std::map<int, std::vector<char> > _failed_buffers;

	fd_set _readSet;
	fd_set _writeSet;
	int _maxFd;
public:
    Server2( void );
    ~Server2();
    void setServConf(std::vector<ServerConfig> &servConf);
    std::map<size_t, std::string> getPorts() const;
    std::vector<ServerConfig> getServConf() const;
    bool findResponse(int sockFd);
    bool findFailedBuffer(int sockFd);
    int start();
};

#endif