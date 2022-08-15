/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:17:10 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/15 17:54:12 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server2.hpp"

Server2::Server2(/* args */) : _maxFd(0)
{
}

Server2::~Server2()
{
}

void Server2::setPorts(std::map<size_t, std::string> &ports)
{
    this->_ports = ports;
}

void Server2::setServConf(std::vector<ServerConfig> &servConf)
{
    this->_servConf = servConf;
}

bool Server2::findResponse(int sockFd) {
    return (this->_responses.find(sockFd) != this->_responses.end());
}

int Server2::start()
{
    // set ports and ips
    for (size_t i = 0; i < this->_servConf.size(); i++)
    {
        _ports.insert(std::make_pair(_servConf[i].getPort(), _servConf[i].getServerIp()));
        _states[i] = State::INIT;
    }

    // init fd_sets
    fd_set tmp_readSet;
    fd_set tmp_writeSet;

    FD_ZERO(&tmp_readSet);
    FD_ZERO(&tmp_writeSet);
    FD_ZERO(&this->_readSet);
    FD_ZERO(&this->_writeSet);

    // create sockets
    for (std::map<size_t, std::string>::iterator it = this->_ports.begin(); it != this->_ports.end(); it++)
    {
        Socket sock(true);
        sock.setPort(it->first);
        sock.setHost(it->second);
        sock.launchSock();
        // add sockets to fd_sets
        FD_SET(sock.getSockFd(), &tmp_readSet);
        if (sock.getSockFd() > this->_maxFd)
            this->_maxFd = sock.getSockFd();

        this->_sockets.push_back(sock);
    }

    // start main loop
    while (1)
    {
        memcpy(&tmp_readSet, &this->_readSet, sizeof(this->_readSet));
        memcpy(&tmp_writeSet, &this->_writeSet, sizeof(this->_writeSet));

        int result = select(this->_maxFd + 1, &tmp_readSet, &tmp_writeSet, NULL, NULL);

        if (result < 0)
            throw std::runtime_error("select() failed");

         // 1. create new clients from server sockets
        for (size_t i = 0; i < this->_sockets.size(); i++) {
            if (_states[i] == State::INIT) {
                socklen_t addr_size = sizeof(_sockets[i].getSockAddr());

                int new_sock_fd = accept(_sockets[i].getSockFd(), (sockaddr *)&_sockets[i].getSockAddr(), &addr_size);
                if (new_sock_fd < 0)
                    throw std::runtime_error("accept() failed");
                Socket _client(false);
                _client.setSockFd(new_sock_fd);
                _client.setSockAddr(_sockets[i].getSockAddr());

                _requests.insert(std::pair<int, Request>(new_sock_fd, Request()));
                _clients.push_back(_client);

                _states[i] = State::ACCEPTED;
            }
        }

        // 2. read from clients, and write to them.
        for (int i = 0; i < this->_clients.size(); i++) {

            // read from client
            if (_states[i] == State::ACCEPTED) {
                if (FD_ISSET(_clients[i].getSockFd(), &tmp_readSet)) {
                    char buffer[BUFFER_SIZE + 1] = {0};
                    ssize_t xrecv = recv(_clients[i].getSockFd(), buffer, BUFFER_SIZE, 0);
                    if (xrecv < 0) {
                        _states[i] = State::RECV_ERROR;
                    } else if (xrecv >= 0) {
                        buffer[xrecv] = '\0';
                        if (xrecv > 0)
                            parseRequest(_requests[_clients[i].getSockFd()], std::string(buffer));
                        if (_requests[_clients[i].getSockFd()].getState() == Request::COMPLETED) {
                            FD_CLR(_clients[i].getSockFd(), &this->_readSet);
                            FD_CLR(_clients[i].getSockFd(), &tmp_readSet);
                            FD_SET(_clients[i].getSockFd(), &this->_writeSet);
                            FD_SET(_clients[i].getSockFd(), &tmp_writeSet);
                            _states[i] = State::READ_REQUEST;
                        }
                    }
                } else {
                    debugPrint(_DEBUG, __FILE__, __LINE__, toString(i));
                    debugPrint(_DEBUG, __FILE__, __LINE__, " : Not Ready to Read.");
                }
            }

            // write to client
            if (_states[i] == State::READ_REQUEST) {

                if (FD_ISSET(_clients[i].getSockFd(), &tmp_writeSet)) {
                    if (!findResponse(_clients[i].getSockFd())) {
                        Response res = ResponseHandler::handleRequests(_requests[_clients[i].getSockFd()], _servConf.front()); // TODO: handle multiple servers
                        _responses[_clients[i].getSockFd()] = res; // TODO: warning: may reset response somehow.
                    }
                    Response& res = _responses[_clients[i].getSockFd()];
                    
                    std::string resStr = res.toString();

                    if (!res.areHeadersSent()) {

                        ssize_t xsend = send(_clients[i].getSockFd(), resStr.c_str(), resStr.length(), 0);

                        if (xsend < 0) {
                            _states[i] = State::SEND_ERROR;
                        } else if (res.isBuffered()) {
                            _states[i] = State::WROTE_HEADERS;
                        } else if (!res.isBuffered()) {
                            _states[i] = State::COMPLETED;
                        }
                    }
                }
            }

            if (_states[i] == State::WROTE_HEADERS) {
                if (FD_ISSET(_clients[i].getSockFd(), &tmp_writeSet)) {
                    if (!findResponse(_clients[i].getSockFd())) {
                        _states[i] = State::ERROR;
                        std::cerr << "FUCK: Response not found (this shouldn't happen)." << std::endl;
                    }

                    char buffer[BUFFER_SIZE + 1] = {0};
                    Response& res = _responses[_clients[i].getSockFd()];
                    size_t xread = res.getNextBuffer(buffer);
                    if (xread > 0) {
                        buffer[xread] = '\0';
                        ssize_t xsend = send(_clients[i].getSockFd(), buffer, xread, 0);
                        if (xsend < 0) {
                            _states[i] = State::SEND_ERROR;
                        }
                    } else {
                        std::cerr << i << ": clear response" << std::endl;
                        _responses.erase(i);
                    }
                }
            }

            if (_states[i] == State::COMPLETED) {
                Response& res = _responses[_clients[i].getSockFd()];
                // TODO: handle segfault in clearAll().
                // res.clearAll();
                _responses.erase(_clients[i].getSockFd());
                _requests.erase(_clients[i].getSockFd());
                FD_CLR(_clients[i].getSockFd(), &this->_writeSet);
                FD_CLR(_clients[i].getSockFd(), &tmp_writeSet);
                close(_clients[i].getSockFd());

                _states[i] = State::INIT;
            }

            if (_states[i] < 0) {
                std::cerr << "Error: State: " << _states[i] << std::endl;
                _responses.erase(_clients[i].getSockFd());
                _requests.erase(_clients[i].getSockFd());
                FD_CLR(_clients[i].getSockFd(), &this->_writeSet);
                FD_CLR(_clients[i].getSockFd(), &tmp_writeSet);
                close(_clients[i].getSockFd());
            }

            // TODO: TEST AND FIX THIS.
        }
    }
}