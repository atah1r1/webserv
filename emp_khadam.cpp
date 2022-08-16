/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emp_khadam.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:17:10 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/16 06:39:54 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server2.hpp"

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
    case SEND_ERROR:
        return "SEND_ERROR";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

Server2::Server2( void ) : _maxFd(0) { }

Server2::~Server2() { }

void Server2::setServConf(std::vector<ServerConfig> &servConf) {
    this->_servConf = servConf;
}

std::map<size_t, std::string> Server2::getPorts() const {
    return this->_ports;
}

std::vector<ServerConfig> Server2::getServConf( void ) const {
    return this->_servConf;
}

bool Server2::findResponse(int sockFd) {
    return (this->_responses.find(sockFd) != this->_responses.end());
}

int Server2::start( void ) {
     struct timeval timeout;
    // --- //
    timeout.tv_sec = 60 * 60 * 24; // 1 day timeout
    timeout.tv_usec = 0;
    size_t _client_index = 0;

    // set ports and ips
    debugPrint(_INFO, __FILE__, __LINE__, "Setting ports and ips");
    for (size_t i = 0; i < this->_servConf.size(); i++) {
        _ports.insert(std::make_pair(_servConf[i].getPort(), _servConf[i].getServerIp()));
        _states[i] = INIT;
    }

    // init fd_sets
    debugPrint(_INFO, __FILE__, __LINE__, "Initializing fd_sets");
    fd_set tmp_readSet;
    fd_set tmp_writeSet;
    FD_ZERO(&this->_readSet);
    FD_ZERO(&this->_writeSet);
    FD_ZERO(&tmp_readSet);
    FD_ZERO(&tmp_writeSet);

    // create sockets
    debugPrint(_INFO, __FILE__, __LINE__, "Creating sockets");
    for (std::map<size_t, std::string>::iterator it = this->_ports.begin(); it != this->_ports.end(); it++) {
        Socket sock(true);
        sock.setPort(it->first);
        sock.setHost(it->second);
        sock.launchSock();

        // add sockets to fd_sets
        FD_SET(sock.getSockFd(), &_readSet);
        if (sock.getSockFd() > this->_maxFd)
            this->_maxFd = sock.getSockFd();

        this->_sockets.push_back(sock);
    }

    // start main loop
    signal(SIGPIPE, SIG_IGN);
    debugPrint(_INFO, __FILE__, __LINE__, "Starting main loop");
    while (1) {
        // 0. copy _Sets to tmp_Sets
        //std::cerr << "LOOP: 0. copy _Sets to tmp_Sets" << std::endl;
        memcpy(&tmp_readSet, &_readSet, sizeof(fd_set));
        memcpy(&tmp_writeSet, &_writeSet, sizeof(fd_set));
        // tmp_readSet = _readSet;
        // tmp_writeSet = _writeSet;

        // 1. select
        std::cerr << C_GREEN "LOOP: 1. WAITING..." C_RESET << std::endl;
        int xselect = select(this->_maxFd + 1, &tmp_readSet, &tmp_writeSet, NULL, &timeout);
        if (xselect < 0) {
            // std::cerr << "LOOP: select error: " << strerror(errno) << std::endl;
            throw std::runtime_error(std::string("select() failed: ") + strerror(errno));
        } else if (xselect == 0) {
            throw std::runtime_error(std::string("select() timeout: ") + strerror(errno));
        }

        //std::cerr << "LOOP: 2. select, FINISHED!" << std::endl;

        // 2. create new clients from server sockets
        //std::cerr << "LOOP: 3. create new clients from server sockets: s:size: " << _sockets.size() << std::endl;
        for (size_t i = 0; i < this->_sockets.size(); i++) {
            //if (_states[i] == INIT) {
                if (FD_ISSET(_sockets[i].getSockFd(), &tmp_readSet)) {
                    std::cerr << "[" << i << "] INIT: accepting socket connections." << std::endl;
                    socklen_t addr_size = sizeof(_sockets[i].getSockAddr());
                    struct sockaddr_in sock_addr = _sockets[i].getSockAddr();
                    int new_sock_fd = accept(_sockets[i].getSockFd(), (struct sockaddr *)&sock_addr, &addr_size);
                    if (new_sock_fd < 0)
                        throw std::runtime_error("accept() failed");
                    Socket _client(false);
                    _client.setSockFd(new_sock_fd);
                    _client.setSockAddr(sock_addr);
                    FD_SET(_client.getSockFd(), &_readSet);
                    FD_SET(_client.getSockFd(), &tmp_readSet);
                    if (_client.getSockFd() > this->_maxFd)
                        this->_maxFd = _client.getSockFd();
                    _requests[new_sock_fd] = Request();
                    _clients.push_back(_client);
                    _states[_client_index++] = ACCEPTED;
                } else {
                    debugPrint(_DEBUG, __FILE__, __LINE__, toString(i) + " : Not Ready to Read.");
                }
            //}
        }

        // 3. read from clients, and write to them.
        //std::cerr << "LOOP: 4. read from clients, and write to them. c:size: " << _clients.size() << std::endl;
        for (size_t i = 0; i < this->_clients.size(); i++) {

            // 4. read from client
            if (_states[i] == ACCEPTED) {
                std::cerr << "[" << i << "] ACCEPTED: read from client." << std::endl;
                if (FD_ISSET(_clients[i].getSockFd(), &tmp_readSet)) {
                    char buffer[BUFFER_SIZE + 1] = {0};
                    ssize_t xrecv = read(_requests.find(_clients[i].getSockFd())->first, buffer, BUFFER_SIZE);
                    //ssize_t xrecv = recv(_clients[i].getSockFd(), buffer, BUFFER_SIZE, 0);
                    if (xrecv < 0) {
                        _states[i] = RECV_ERROR;
                    } else if (xrecv >= 0) {
                        buffer[xrecv] = '\0';
                        if (xrecv > 0)
                            parseRequest(_requests[_requests.find(_clients[i].getSockFd())->first], std::string(buffer));
                        if (_requests[_requests.find(_clients[i].getSockFd())->first].getState() == Request::COMPLETED) {

                            // FD_CLR(_clients[i].getSockFd(), &this->_readSet);
                            // FD_CLR(_clients[i].getSockFd(), &tmp_readSet);
                            // FD_SET(_clients[i].getSockFd(), &this->_writeSet);
                            // FD_SET(_clients[i].getSockFd(), &tmp_writeSet);
                            FD_CLR(_requests.find(_clients[i].getSockFd())->first, &this->_readSet);
                            FD_CLR(_requests.find(_clients[i].getSockFd())->first, &tmp_readSet);
                            FD_SET(_requests.find(_clients[i].getSockFd())->first, &this->_writeSet);
                            FD_SET(_requests.find(_clients[i].getSockFd())->first, &tmp_writeSet);
                            _states[i] = READ_REQUEST;
                            std::cerr << "[" << i << "] HANDING REQUEST . PATH: [" << _requests[_clients[i].getSockFd()].getPath() << "]" << std::endl;
                        }
                    }
                } else {
                    debugPrint(_DEBUG, __FILE__, __LINE__, toString(i) + " : Not Ready to Read.");
                }
            }

            // 5. write headers to client
            if (_states[i] == READ_REQUEST) {
                std::cerr << "[" << i << "] READ_REQUEST: write headers to client." << std::endl;
                if (FD_ISSET(_clients[i].getSockFd(), &tmp_writeSet)) {
                    if (!findResponse(_requests.find(_clients[i].getSockFd())->first)) {
                        Response res = ResponseHandler::handleRequests(_requests[_requests.find(_clients[i].getSockFd())->first], _servConf.front()); // TODO: handle multiple servers
                        _responses[_requests.find(_clients[i].getSockFd())->first] = res; // TODO: warning: may reset response somehow.
                    }
                    Response& res = _responses[_requests.find(_clients[i].getSockFd())->first];
                    
                    std::string resStr = res.toString();

                    if (!res.areHeadersSent()) {

                        // ssize_t xsend = send(_clients[i].getSockFd(), resStr.c_str(), resStr.length(), 0);
                        ssize_t xsend = write(_requests.find(_clients[i].getSockFd())->first, resStr.c_str(), resStr.length());

                        if (xsend < 0) {
                            _states[i] = SEND_ERROR;
                        } else if (res.isBuffered()) {
                            _states[i] = WROTE_HEADERS;
                        } else if (!res.isBuffered()) {
                            _states[i] = COMPLETED;
                        }
                    }
                } else {
                    debugPrint(_DEBUG, __FILE__, __LINE__, toString(i) + " : Not Ready to Write.");
                }
            }

            // 6. write body to client
            if (_states[i] == WROTE_HEADERS) {
                std::cerr << "[" << i << "] WROTE_HEADERS: write body to client." << std::endl;
                //if (FD_ISSET(_clients[i].getSockFd(), &tmp_writeSet)) {
                    if (!findResponse(_requests.find(_clients[i].getSockFd())->first)) {
                        _states[i] = ERROR;
                        std::cerr << "[" << i << "] FUCK: Response not found (this shouldn't happen)." << std::endl;
                    }

                    char buffer[BUFFER_SIZE + 1] = {0};
                    Response& res = _responses[_requests.find(_clients[i].getSockFd())->first];
                    size_t xread = res.getNextBuffer(buffer);
                    if (xread > 0) {
                        buffer[xread] = '\0';
                        //ssize_t xsend = send(_clients[i].getSockFd(), buffer, xread, 0);
                        ssize_t xsend = write(_requests.find(_clients[i].getSockFd())->first, buffer, xread);
                        if (xsend < 0) {
                            _states[i] = SEND_ERROR;
                        }
                    } else {
                        _states[i] = COMPLETED;
                    }
                // } else {
                //     debugPrint(_DEBUG, __FILE__, __LINE__, toString(i) + " : Not Ready to Write.");
                // }
            }

            // 7. close client
            if (_states[i] == COMPLETED) {
                std::cerr << "[" << i << "] COMPLETED: close client." << std::endl;
                FD_CLR(_requests.find(_clients[i].getSockFd())->first, &this->_writeSet);
                FD_CLR(_requests.find(_clients[i].getSockFd())->first, &tmp_writeSet);
                int xclose = close(_requests.find(_clients[i].getSockFd())->first);
                if (xclose < 0)
                    throw std::runtime_error("Error closing client socket.");
                Response& res = _responses[_requests.find(_clients[i].getSockFd())->first];
                res.clearAll();
                _responses.erase(_requests.find(_clients[i].getSockFd())->first);
                _requests.erase(_requests.find(_clients[i].getSockFd())->first);
                _states[i] = INIT;
            }

            // 8. errors
            if (_states[i] < 0) {
                std::cerr << "[" << i << "] ERROR: " << getStateStr(_states[i]) << ": error: " << strerror(errno) << std::endl;
                if (_states[i] == RECV_ERROR) {
                    FD_CLR(_requests.find(_clients[i].getSockFd())->first, &this->_readSet);
                    FD_CLR(_requests.find(_clients[i].getSockFd())->first, &tmp_readSet);
                } else {
                    FD_CLR(_requests.find(_clients[i].getSockFd())->first, &this->_writeSet);
                    FD_CLR(_requests.find(_clients[i].getSockFd())->first, &tmp_writeSet);
                }
                int xclose = close(_requests.find(_clients[i].getSockFd())->first);
                if (xclose < 0)
                    throw std::runtime_error("Error closing client socket.");
                Response& res = _responses[_requests.find(_clients[i].getSockFd())->first];
                res.clearAll();
                _responses.erase(_requests.find(_clients[i].getSockFd())->first);
                _requests.erase(_requests.find(_clients[i].getSockFd())->first);
                _states[i] = INIT;
            }

            // TODO: TEST AND FIX THIS.
        }
    }
    return 0;
}
