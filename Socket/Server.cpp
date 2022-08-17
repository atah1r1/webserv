/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:17:10 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/17 22:03:58 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string getStateStr(State s) {
    switch (s)
    {
    case INIT:
        return "INIT";
    case ACCEPTED:
        return "ACCEPTED";
    case READ_REQUEST:
        return "READ_REQUEST";
    case WROTE_HEADERS:
        return "WROTE_HEADERS";
    case WROTE_BODY:
        return "WROTE_BODY";
    case COMPLETED:
        return "COMPLETED";
    case CLOSED:
        return "CLOSED";
    case RECV_ERROR:
        return "RECV_ERROR";
    case SEND_HEADER_ERROR:
        return "SEND_HEADER_ERROR";
    case SEND_BODY_ERROR:
        return "SEND_BODY_ERROR";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

Server::Server( void ) : _maxFd(0) { }

Server::~Server() { }

void Server::setServConf(std::vector<ServerConfig> &servConf) {
    this->_servConf = servConf;
}

std::map<size_t, std::string> Server::getPorts() const {
    return this->_ports;
}

std::vector<ServerConfig> Server::getServConf( void ) const {
    return this->_servConf;
}

bool Server::findResponse(int sockFd) {
    return (this->_responses.find(sockFd) != this->_responses.end());
}

int Server::start( void ) {
    struct timeval timeout;
    timeout.tv_sec = 60 * 60;
    timeout.tv_usec = 0;

    // ======================= set ports and ips ======================= //
    for (size_t i = 0; i < this->_servConf.size(); i++) {
        _ports.insert(std::make_pair(_servConf[i].getPort(), _servConf[i].getServerIp()));
    }

    // ======================= init fd_sets ======================= //
    fd_set tmp_readSet;
    fd_set tmp_writeSet;
    FD_ZERO(&this->_readSet);
    FD_ZERO(&this->_writeSet);
    FD_ZERO(&tmp_readSet);
    FD_ZERO(&tmp_writeSet);

    // ======================= create sockets ======================= //
    for (std::map<size_t, std::string>::iterator it = this->_ports.begin(); it != this->_ports.end(); it++) {
        Socket sock(true);
        sock.setPort(it->first);
        sock.setHost(it->second);
        sock.launchSock();

        FD_SET(sock.getSockFd(), &_readSet);
        if (sock.getSockFd() > this->_maxFd)
            this->_maxFd = sock.getSockFd();

        this->_sockets.push_back(sock);
    }

    // ======================= start main loop ======================= //
    while (true) {
        // ======================= 0. copy _Sets to tmp_Sets ======================= //
        memcpy(&tmp_readSet, &_readSet, sizeof(fd_set));
        memcpy(&tmp_writeSet, &_writeSet, sizeof(fd_set));

        // ======================= 1. select ======================= //
        int xselect = select(this->_maxFd + 1, &tmp_readSet, &tmp_writeSet, NULL, &timeout);
        if (xselect < 0) {
            throw std::runtime_error(std::string("select() failed: ") + strerror(errno));
        } else if (xselect == 0) {
            throw std::runtime_error(std::string("select() timeout: ") + strerror(errno));
        }

        // ======================= 2. create new clients from server sockets ======================= //
        for (size_t i = 0; i < this->_sockets.size(); i++) {
            if (FD_ISSET(_sockets[i].getSockFd(), &tmp_readSet)) {
                socklen_t addr_size = sizeof(_sockets[i].getSockAddr());
                struct sockaddr_in sock_addr = _sockets[i].getSockAddr();
                int new_sock_fd = accept(_sockets[i].getSockFd(), (struct sockaddr *)&sock_addr, &addr_size);
                if (new_sock_fd < 0)
                    throw std::runtime_error("accept() failed");
                Socket _client(false);
                _client.setSockFd(new_sock_fd);
                _client.setSockAddr(sock_addr);
                FD_SET(new_sock_fd, &_readSet);
                FD_SET(new_sock_fd, &tmp_readSet);
                if (_client.getSockFd() > this->_maxFd)
                    this->_maxFd = _client.getSockFd();
                _requests[new_sock_fd] = Request();
                _clients.push_back(_client);
                _states.push_back(ACCEPTED);
            }
        }

        // ======================= 3. read from clients, and write to them. ======================= //
        for (size_t i = 0; i < this->_clients.size(); i++) {
            int fd = _clients[i].getSockFd();

            // ======================= 4. read from client ======================= //
            if (_states[i] == ACCEPTED && FD_ISSET(fd, &tmp_readSet)) {
                char buffer[BUFFER_SIZE + 1] = {0};
                ssize_t xrecv = recv(fd, buffer, BUFFER_SIZE, 0);
                if (xrecv < 0) {
                    std::cerr << "[" << i << "] RECV_ERROR: SKIPPING: " << strerror(errno) << std::endl;
                } else if (xrecv >= 0) {
                    buffer[xrecv] = '\0';
                    if (xrecv > 0)
                        parseRequest(_requests[fd], std::string(buffer));
                    if (_requests[fd].getState() == Request::COMPLETED) {
                        FD_CLR(fd, &this->_readSet);
                        FD_CLR(fd, &tmp_readSet);
                        FD_SET(fd, &this->_writeSet);
                        FD_SET(fd, &tmp_writeSet);
                        _states[i] = READ_REQUEST;
                    }
                }
                // whatever happens move to next fd.
                continue;
            }

            // ======================= 5. write headers to client ======================= //
            if (_states[i] == READ_REQUEST && FD_ISSET(fd, &tmp_writeSet)) {
                if (!findResponse(fd)) {
                    Response res = ResponseHandler::handleRequests(_requests[fd], _servConf.front()); // TODO: handle multiple servers
                    _responses.insert(std::pair<int, Response>(fd, res));
                }
                Response& res = _responses[fd];
                if (!res.areHeadersSent()) {
                    std::string resStr = res.toString();
                    if (resStr.length() == 0) {
                        throw std::runtime_error("Response is empty.");
                    }
                    ssize_t xsend = send(fd, resStr.c_str(), resStr.length(), 0);
                    if (xsend < 0) {
                        _states[i] = ERROR;
                        std::cerr << "[" << i << "] SEND_HEADER_ERROR: STOPPING" << std::endl;
                    } else if (res.isBuffered()) {
                        _states[i] = WROTE_HEADERS;
                        res.setHeadersSent(true);
                        continue;
                    } else if (!res.isBuffered()) {
                        _states[i] = COMPLETED;
                        res.setHeadersSent(true);
                    }
                }
            }

            // ======================= 6. write body to client ======================= //
            if (_states[i] == WROTE_HEADERS && FD_ISSET(fd, &tmp_writeSet)) {
                if (!findResponse(fd)) {
                    _states[i] = ERROR;
                    std::cerr << "[" << i << "] FUCK: Response not found (this shouldn't happen)." << std::endl;
                }
                char buffer[BUFFER_SIZE] = {0};
                Response& res = _responses[fd];
                size_t xread = res.getNextBuffer(buffer);

                if (xread > 0) {
                    ssize_t xsend = send(fd, buffer, xread, 0);
                    if (xsend < 0) {
                        _states[i] = ERROR;
                        std::cerr << "[" << i << "] SEND_BODY_ERROR: STOPPING" << std::endl;
                    } else {
                        continue;
                    }
                } else {
                    _states[i] = COMPLETED;
                }
            }

            // ======================= 8. errors ======================= //
            if (_states[i] == ERROR) {
                std::cerr << "[" << i << "] COMPLETED: ERROR" << std::endl;
                FD_CLR(fd, &this->_writeSet);
                FD_CLR(fd, &tmp_writeSet);
                _responses[fd].clearAll();
                _responses.erase(fd);
                _requests.erase(fd);
                _clients.erase(_clients.begin() + i);
                _states.erase(_states.begin() + i);
                continue;
            }

            // ======================= 7. close client ======================= //
            if (_states[i] == COMPLETED) {
                if (_requests[fd].getHeader(H_CONNECTION) == "keep-alive") {
                    std::cerr << "[" << i << "] COMPLETED: KEEP-ALIVE" << std::endl;
                    FD_CLR(fd, &this->_writeSet);
                    FD_CLR(fd, &tmp_writeSet);
                    FD_SET(fd, &this->_readSet);
                    FD_SET(fd, &tmp_readSet);
                    _states[i] = ACCEPTED;
                    _responses[fd].clearAll();
                    _responses.erase(fd);
                    _requests.erase(fd);
                    _requests[fd] = Request();
                } else {
                    std::cerr << "[" << i << "] COMPLETED: CLOSE" << std::endl;
                    FD_CLR(fd, &this->_writeSet);
                    FD_CLR(fd, &tmp_writeSet);
                    int xclose = close(fd);
                    if (xclose < 0)
                        throw std::runtime_error("Error closing client socket.");
                    _responses[fd].clearAll();
                    _responses.erase(fd);
                    _requests.erase(fd);
                    _clients.erase(_clients.begin() + i);
                    _states.erase(_states.begin() + i);
                }
            }
 
        }
    }
    return 0;
}
