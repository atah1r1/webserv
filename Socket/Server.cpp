/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:17:10 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/18 01:07:39 by ehakam           ###   ########.fr       */
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

void Server::setPorts( void ) {
    for (size_t i = 0; i < this->_servConf.size(); i++) {
        _ports.insert(std::make_pair(_servConf[i].getPort(), _servConf[i].getServerIp()));
    }
}

std::map<size_t, std::string> Server::getPorts() const {
    return this->_ports;
}

std::vector<ServerConfig> Server::getServConf( void ) const {
    return this->_servConf;
}

void Server::initSets(fd_set& tmpReadSet, fd_set& tmpWriteSet) {
    FD_ZERO(&this->_readSet);
    FD_ZERO(&this->_writeSet);
    FD_ZERO(&tmpReadSet);
    FD_ZERO(&tmpWriteSet);
}

bool Server::findResponse(int sockFd) {
    return (this->_responses.find(sockFd) != this->_responses.end());
}

void Server::setupServerSockets( void ) {
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
}

void Server::acceptClients( fd_set& tmpReadSet ) {
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        if (FD_ISSET(_sockets[i].getSockFd(), &tmpReadSet)) {
            socklen_t addr_size = sizeof(_sockets[i].getSockAddr());
            struct sockaddr_in sock_addr = _sockets[i].getSockAddr();
            int new_sock_fd = accept(_sockets[i].getSockFd(), (struct sockaddr *)&sock_addr, &addr_size);
            if (new_sock_fd < 0)
                throw std::runtime_error("accept() failed");
            Socket _client(false);
            _client.setSockFd(new_sock_fd);
            _client.setSockAddr(sock_addr);
            FD_SET(new_sock_fd, &_readSet);
            if (_client.getSockFd() > this->_maxFd)
                this->_maxFd = _client.getSockFd();
            _requests[new_sock_fd] = Request();
            _clients.push_back(_client);
            _states.push_back(ACCEPTED);
        }
    }
}

void Server::readRequest( int& fd, size_t& index, fd_set& tmpReadSet, fd_set& tmpWriteSet ) {
    char buffer[BUFFER_SIZE + 1] = {0};
    ssize_t xrecv = recv(fd, buffer, BUFFER_SIZE, 0);
    if (xrecv < 0) {
        std::cerr << "[" << index << "] RECV_ERROR: STOPPING: " << strerror(errno) << std::endl;
        _states[index] = ERROR;
    } else if (xrecv >= 0) {
        buffer[xrecv] = '\0';
        if (xrecv > 0)
            parseRequest(_requests[fd], std::string(buffer));
        if (_requests[fd].getState() == Request::COMPLETED) {
            FD_CLR(fd, &this->_readSet);
            FD_CLR(fd, &tmpReadSet);
            FD_SET(fd, &this->_writeSet);
            FD_SET(fd, &tmpWriteSet);
            _states[index] = READ_REQUEST;
            printRequest(_requests[fd]);
        }
    }
}

void Server::writeResponseHeaders( int& fd, size_t& index ) {
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
            _states[index] = ERROR;
            std::cerr << "[" << index << "] SEND_HEADER_ERROR: STOPPING" << std::endl;
        } else if (res.isBuffered()) {
            _states[index] = WROTE_HEADERS;
            res.setHeadersSent(true);
        } else if (!res.isBuffered()) {
            _states[index] = COMPLETED;
            res.setHeadersSent(true);
        }
    }
}

void Server::writeResponseBody( int& fd, size_t& index ) {
    if (!findResponse(fd)) {
        _states[index] = ERROR;
        std::cerr << "[" << index << "] CACHED_RESPONSE_NOT_FOUND: (this shouldn't happen)." << std::endl;
    }
    char buffer[BUFFER_SIZE] = {0};
    Response& res = _responses[fd];
    size_t xread = res.getNextBuffer(buffer);

    if (xread > 0) {
        ssize_t xsend = send(fd, buffer, xread, 0);
        if (xsend < 0) {
            _states[index] = ERROR;
            std::cerr << "[" << index << "] SEND_BODY_FAILED: CLIENT DISCONNECTED" << std::endl;
        }
    } else {
        _states[index] = COMPLETED;
    }
}

void Server::terminateClient( int& fd, size_t& index, fd_set& tmpReadSet, fd_set& tmpWriteSet ) {
    FD_CLR(fd, &this->_readSet);
    FD_CLR(fd, &tmpReadSet);
    FD_CLR(fd, &this->_writeSet);
    FD_CLR(fd, &tmpWriteSet);
    int xclose = close(fd);
    if (xclose < 0)
        throw std::runtime_error("Error closing client socket.");
    _responses[fd].clearAll();
    _responses.erase(fd);
    _requests.erase(fd);
    _clients.erase(_clients.begin() + index);
    _states.erase(_states.begin() + index);
}

void Server::keepAlive( int& fd, size_t& index, fd_set& tmpReadSet, fd_set& tmpWriteSet ) {
    FD_CLR(fd, &this->_writeSet);
    FD_CLR(fd, &tmpWriteSet);
    FD_SET(fd, &this->_readSet);
    FD_SET(fd, &tmpReadSet);
    _states[index] = ACCEPTED;
    _responses[fd].clearAll();
    _responses.erase(fd);
    _requests.erase(fd);
    _requests[fd] = Request();
}

void Server::listen( fd_set& tmpReadSet, fd_set& tmpWriteSet ) {

    for (size_t i = 0; i < this->_clients.size(); i++) {
        int fd = _clients[i].getSockFd();

        // ======================= read from / write to client ======================= //
        if (_states[i] == ACCEPTED && FD_ISSET(fd, &tmpReadSet)) {
            readRequest(fd, i, tmpReadSet, tmpWriteSet);

        } else if (_states[i] == READ_REQUEST && FD_ISSET(fd, &tmpWriteSet)) {
            writeResponseHeaders(fd, i);

        } else if (_states[i] == WROTE_HEADERS && FD_ISSET(fd, &tmpWriteSet)) {
            writeResponseBody(fd, i);

        }

        // ======================= close / keep client alive ======================= //
        if (_states[i] == ERROR) {
            try {
                terminateClient(fd, i, tmpReadSet, tmpWriteSet);
            } catch (std::exception& e) {
                throw;
            }
        } else if (_states[i] == COMPLETED) {
            
            if (_requests[fd].getHeader(H_CONNECTION) == "keep-alive") {
                std::cerr << "[" << i << "] COMPLETED: KEEP-ALIVE" << std::endl;
                keepAlive(fd, i, tmpReadSet, tmpWriteSet);
            } else {
                std::cerr << "[" << i << "] COMPLETED: CLOSE" << std::endl;
                try {
                    terminateClient(fd, i, tmpReadSet, tmpWriteSet);
                } catch (std::exception& e) {
                    throw;
                }
            }
        }

    }
}

int Server::start( void ) {
    fd_set tmpReadSet;
    fd_set tmpWriteSet;

    initSets(tmpReadSet, tmpWriteSet);
    setPorts();
    setupServerSockets();

    while (true) {
        memcpy(&tmpReadSet, &_readSet, sizeof(fd_set));
        memcpy(&tmpWriteSet, &_writeSet, sizeof(fd_set));

        int xselect = select(this->_maxFd + 1, &tmpReadSet, &tmpWriteSet, NULL, NULL);
        if (xselect < 0) {
            throw std::runtime_error(std::string("select() failed: ") + strerror(errno));
        } else if (xselect == 0) {
            throw std::runtime_error(std::string("select() timeout: ") + strerror(errno));
        }
        acceptClients(tmpReadSet);

        try {
            listen(tmpReadSet, tmpWriteSet);
        } catch (std::exception& e) {
            throw;
        }
    }
    return 0;
}
