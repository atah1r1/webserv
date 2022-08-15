/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 12:24:52 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/15 15:07:15 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _maxFd(-1)
{
}

Server::~Server()
{
    this->clean();
}

void Server::setPorts(std::map<size_t, std::string> &ports)
{
    this->_ports = ports;
}

void Server::setServConf(std::vector<ServerConfig> &servConf)
{
    this->_servConf = servConf;
}

void Server::startServerSockets()
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

void Server::addToSet(int fd, fd_set &set)
{
    FD_SET(fd, &set);

    // update our maxFd if the new fd is greater than madFd
    if (fd > this->_maxFd)
        this->_maxFd = fd;
}

void Server::deleteFromSet(int fd, fd_set &set)
{
    FD_CLR(fd, &set);
}

void Server::fillSockSet()
{
    // reset our sets
    FD_ZERO(&this->_readSet);
    FD_ZERO(&this->_writeSet);

    // add server sockets to the read set
    for (size_t i = 0; i < this->_sockets.size(); i++)
        addToSet(this->_sockets[i]->getSockFd(), this->_readSet);
}

std::map<int, Response> _responses;
std::map<int, int> _done;

std::pair<bool, Response> findResponse(int i) {
    std::map<int, Response>::iterator it = _responses.find(i);
    if (it == _responses.end()) return std::make_pair<bool, Response>(false, Response());
    return std::make_pair<bool, Response>(true, it->second);
}

void Server::performSelect() {

    char buffer[BUFFER_SIZE] = {0};
    fd_set tmpReadSet = this->_readSet;
    fd_set tmpWriteSet = this->_writeSet;
    size_t i = 0;
    ssize_t _ret = 0;
    size_t xread = 0;

    // wait for events in ReadSet and WriteSet
    int result = select(this->_maxFd + 1, &tmpReadSet, &tmpWriteSet, NULL, NULL);

    if (result <= 0) return;

    while (i < _sockets.size()) {

        std::cerr << "Iteration: " << i << " - " << _sockets.size() << std::endl;

        if (FD_ISSET(this->_sockets[i]->getSockFd(), &tmpReadSet))
        {
            if (this->_sockets[i]->isServSock()) {
                std::cerr << i << ": accept new client." << std::endl;
                acceptNewClient(this->_sockets[i]);
            } else {
                std::cerr << i << ": read request." << std::endl;
                _ret = handleClient(this->_sockets[i]);
                if (_ret > 0)
                    std::cerr << i << ": read request not finished continue." << std::endl;
                if (_ret < 0) {
                    std::cerr << i << ": handleClient failed." << std::endl;
                }
            }
        } else {
            std::cerr << i << ": not ready to read" << std::endl;
        }

        if (FD_ISSET(this->_sockets[i]->getSockFd(), &tmpWriteSet)) {

            std::cerr << i << ": finding response." << std::endl;

            std::pair<bool, Response> p = findResponse(i);

            if (!p.first) {
                 p.second = ResponseHandler::handleRequests(this->_clients[this->_sockets[i]->getSockFd()], this->_servConf.front());
                 _responses.insert(std::pair<int, Response>(i, p.second));
            }

            Response r = p.second;

            std::string response = r.toString();

            if (_ret >= 0 && !r.areHeadersSent()) {
                std::cerr << i << ": send res headers" << std::endl;
                _ret = send(this->_sockets[i]->getSockFd(), response.c_str(), response.size(), 0);
                if (_ret >= 0)
                    r.setHeadersSent(true);
                    _responses[i].setHeadersSent(true);
                if (_ret < 0) {
                    std::cerr << i << ": send Headers failed." << std::endl;
                }
            }

            if (_ret >= 0 && r.isBuffered()) {

                std::cerr << i << ": read buffer." << std::endl;
                xread = r.getNextBuffer(buffer);

                if (xread > 0) {
                    std::cerr << i << ": send buffer" << std::endl;
                    _ret = send(this->_sockets[i]->getSockFd(), buffer, xread, 0);
                    if (_ret < 0) {
                        std::cerr << i << ": send buffer failed." << std::endl;
                    }
                } else {
                    std::cerr << i << ": clear response" << std::endl;
                     _responses.erase(i);
                }
            } else {
                 std::cerr << i << ": clear response 2" << std::endl;
                _responses.erase(i);
            }

            // on send error.
            if (_ret == -1) {
                std::cout << "send error" << std::endl;
                _responses.erase(i);
                deleteFromSet(this->_sockets[i]->getSockFd(), this->_readSet);
                deleteFromSet(this->_sockets[i]->getSockFd(), this->_writeSet);
                this->_clients.erase(this->_sockets[i]->getSockFd());
                this->_sockets[i]->m_close();
                std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), this->_sockets[i]);
                if (position != this->_sockets.end()) {
                    delete (*position);
                    this->_sockets.erase(position);
                }
                ++i;
                continue;
            }

            // response not fully sent yet.
            if (!r.areHeadersSent() || (r.isBuffered() && xread > 0)) {
                ++i;
                continue; 
            }

            // response fully sent.
            if (this->_clients[this->_sockets[i]->getSockFd()].getHeader("Connection") != "close") {
                this->_clients.insert(std::make_pair(this->_sockets[i]->getSockFd(), Request()));
                deleteFromSet(this->_sockets[i]->getSockFd(), this->_writeSet);
                addToSet(this->_sockets[i]->getSockFd(), this->_readSet);
                 _responses.erase(i);
            } else {
                deleteFromSet(this->_sockets[i]->getSockFd(), this->_writeSet);
                deleteFromSet(this->_sockets[i]->getSockFd(), this->_readSet);
                this->_clients.erase(this->_sockets[i]->getSockFd());
                this->_sockets[i]->m_close();
                std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), this->_sockets[i]);
                if (position != this->_sockets.end()) {
                    delete (*position);
                    this->_sockets.erase(position);
                }
                 _responses.erase(i);
            }
        } else {
            std::cerr << i << ": not ready to write" << std::endl;
        }
        // reset i to 0 when full circle;
        ++i;
    }
}

// void Server::performSelect()
// {
//     fd_set tmpReadSet = this->_readSet;
//     fd_set tmpWriteSet = this->_writeSet;
//     char buffer[BUFFER_SIZE] = {0};
//     // wait for events in ReadSet and WriteSet
//     int result = select(this->_maxFd + 1, &tmpReadSet, &tmpWriteSet, NULL, NULL);
//     if (result > 0)
//     {
//         for (size_t i = 0; i < this->_sockets.size();)
//         {
//             // check if a file descriptor is ready for read
//             if (FD_ISSET(this->_sockets[i]->getSockFd(), &tmpReadSet))
//             {
//                 if (this->_sockets[i]->isServSock())
//                     acceptNewClient(this->_sockets[i]);
//                 else
//                     handleClient(this->_sockets[i]);
//             }
//             // check if a file descriptor is ready for write
//             if (FD_ISSET(this->_sockets[i]->getSockFd(), &tmpWriteSet))
//             {
//                 Response r = ResponseHandler::handleRequests(this->_clients[this->_sockets[i]->getSockFd()], this->_servConf.front());
//                 // send(client->getSockFd(), hello.c_str(), strlen(hello.c_str()), 0);
//                 std::string response = r.toString();
//                 // send Headers to client
//                 int size = 0;
//                 if (FD_ISSET(this->_sockets[i]->getSockFd(), &tmpWriteSet))
//                     size = send(this->_sockets[i]->getSockFd(), response.c_str(), response.size(), 0);
//                 // send chunked body response chunked
//                 size_t len = 0;
//                 if (r.isBuffered())
//                 {
//                     if (FD_ISSET(this->_sockets[i]->getSockFd(), &tmpWriteSet)) {
//                         len = r.getNextBuffer(buffer);
//                         size = send(this->_sockets[i]->getSockFd(), buffer, len, 0);
//                         bzero(buffer, BUFFER_SIZE);
//                     } 
//                     // if (x < 0) {
//                     //     debugPrint(_ERROR, __FILE__, __LINE__, strerror(errno));
//                     // } else {
//                     //     debugPrint(_INFO, __FILE__, __LINE__, "Success!");
//                     // }
//                 }
//                 if (size >= 0 && len > 0) continue;
//                 if (size == -1)
//                 {
//                     std::cout << "send error" << std::endl;
//                     deleteFromSet(this->_sockets[i]->getSockFd(), this->_readSet);
//                     deleteFromSet(this->_sockets[i]->getSockFd(), this->_writeSet);
//                     this->_clients.erase(this->_sockets[i]->getSockFd());
//                     this->_sockets[i]->m_close();
//                     std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), this->_sockets[i]);
//                     if (position != this->_sockets.end())
//                     {
//                         delete (*position);
//                         this->_sockets.erase(position);
//                     }
//                     continue;
//                 }
//                 if (this->_clients[this->_sockets[i]->getSockFd()].getHeader("Connection") == "keep-alive")
//                 {
//                     this->_clients.insert(std::make_pair(this->_sockets[i]->getSockFd(), Request()));
//                     deleteFromSet(this->_sockets[i]->getSockFd(), this->_writeSet);
//                     addToSet(this->_sockets[i]->getSockFd(), this->_readSet);
//                 }
//                 else
//                 {
//                     deleteFromSet(this->_sockets[i]->getSockFd(), this->_writeSet);
//                     deleteFromSet(this->_sockets[i]->getSockFd(), this->_readSet);
//                     this->_clients.erase(this->_sockets[i]->getSockFd());
//                     this->_sockets[i]->m_close();
//                     std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), this->_sockets[i]);
//                     if (position != this->_sockets.end())
//                     {
//                         delete (*position);
//                         this->_sockets.erase(position);
//                     }
//                 }
//             }
//             ++i;
//             if (i == this->_sockets.size()) i = 0;
//         }
//     }
// }

void Server::acceptNewClient(Socket *sock)
{
    // accept connection on server socket and get fd for new Client
    int newClient = accept(sock->getSockFd(), 0, 0);
    Socket *client = new Socket(false);
    client->setSockFd(newClient);

    client->setSockAddr(sock->getSockAddr());
    std::cout << "new client: " << newClient << std::endl;
    this->_clients.insert(std::make_pair(newClient, Request()));

    this->_sockets.push_back(client);

    // add our client to read set
    std::cout << "New client connected: " << client->getSockFd() << std::endl;
    addToSet(client->getSockFd(), this->_readSet);
    sock->setAccepted(true);
}

// void reset(int newClient)
// {
//     this->_clients.insert(std::make_pair(newClient, Request()));
// }

int Server::handleClient(Socket *client)
{
    char buff[MAX_BUFFER_SIZE] = {0};

    // receive data from client
    int size = recv(client->getSockFd(), buff, MAX_BUFFER_SIZE, 0);

    // if an error occured or when a stream socket peer has performed a shutdown.
    if (size == -1 || size == 0)
    {
        std::cout << "ENTERED HERE: HANDLE CLIENT: " << client->getSockFd() << std::endl;
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
        return size;
    }

    // send to parser and check return value if reading is complete
    if (size > 0)
    {
        std::string newStr = std::string(buff, size);
        parseRequest(this->_clients[client->getSockFd()], newStr);
        bool isCompleted = this->_clients[client->getSockFd()].getState() == Request::COMPLETED;
        if (isCompleted)
        {
            std::cout << "Request completed" << std::endl;
            std::cout << "Connection: " << this->_clients[client->getSockFd()].getHeader("Connection") << std::endl;
            client->updateConnection(this->_clients[client->getSockFd()].getHeader("Connection") == "keep-alive");
            deleteFromSet(client->getSockFd(), this->_readSet);
            addToSet(client->getSockFd(), this->_writeSet);
            return 0;
        }
    }
    return size;
}

// void Server::sendResponse(Socket *client)
// {
//     Response r = ResponseHandler::handleRequests(this->_clients[client->getSockFd()], this->_servConf.front());
//     // send(client->getSockFd(), hello.c_str(), strlen(hello.c_str()), 0);
//     std::string response = r.toString();
//     // send Headers to client
//     int size = send(client->getSockFd(), response.c_str(), response.size(), 0);
//     // send chunked body response chunked
//     char buffer[BUFFER_SIZE] = {0};
//     size_t len = 0;
//     if (r.isBuffered())
//     {
//         size_t s = 0;
//         while ((len = r.getNextBuffer(buffer)) > 0)
//         {
//             size = send(client->getSockFd(), buffer, len, 0);
//             bzero(buffer, BUFFER_SIZE);
//             if (size == -1) break;
//             // if (x < 0) {
//             //     debugPrint(_ERROR, __FILE__, __LINE__, strerror(errno));
//             // } else {
//             //     debugPrint(_INFO, __FILE__, __LINE__, "Success!");
//             // }
//             s += len;
//         }
//         // std::cout << "Sent chunk " << s << " of size " << len << std::endl;
//     }
//     if (size == -1)
//     {
//         std::cout << "send error" << std::endl;
//         deleteFromSet(client->getSockFd(), this->_readSet);
//         deleteFromSet(client->getSockFd(), this->_writeSet);
//         this->_clients.erase(client->getSockFd());
//         client->m_close();
//         std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), client);
//         if (position != this->_sockets.end())
//         {
//             delete (*position);
//             this->_sockets.erase(position);
//         }
//         return;
//     }
//     if (this->_clients[client->getSockFd()].getHeader("Connection") == "keep-alive")
//     {
//         this->_clients.insert(std::make_pair(client->getSockFd(), Request()));
//         deleteFromSet(client->getSockFd(), this->_writeSet);
//         addToSet(client->getSockFd(), this->_readSet);
//     }
//     else
//     {
//         deleteFromSet(client->getSockFd(), this->_writeSet);
//         deleteFromSet(client->getSockFd(), this->_readSet);
//         this->_clients.erase(client->getSockFd());
//         client->m_close();
//         std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), client);
//         if (position != this->_sockets.end())
//         {
//             delete (*position);
//             this->_sockets.erase(position);
//         }
//     }
//     // deleteFromSet(client->getSockFd(), this->_writeSet);
//     // this->_clients.erase(client->getSockFd());
//     // client->m_close();
//     // std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), client);
//     // if (position != this->_sockets.end())
//     // {
//     //     delete (*position);
//     //     this->_sockets.erase(position);
//     // }
// }

void Server::clean()
{
    for (size_t i = 0; i < this->_sockets.size(); i++)
    {
        this->_sockets[i]->m_close();
        delete _sockets[i];
    }
    this->_sockets.clear();
}

int Server::start(std::vector<ServerConfig> servers)
{
    Server server;
    std::map<size_t, std::string> ports;
    for (size_t i = 0; i < servers.size(); i++)
        ports.insert(std::make_pair(servers[i].getPort(), servers[i].getServerIp()));
    // for(std::map<size_t, std::string>::iterator it = ports.begin(); it != ports.end(); it++)
    // {
    //     std::cout << "Starting server on port " << it->first << "..." << std::endl;
    //     std::cout << "Starting server on ip  " << it->second << "..." << std::endl;
    // }
    server.setPorts(ports);
    server.setServConf(servers);
    server.startServerSockets();
    server.fillSockSet();
    std::cout << "Server started it can accept connections..." << std::endl;
    signal(SIGPIPE, SIG_IGN);
    while (1)
    {
        // FIX: Broken pipe signal handler
        server.performSelect();
        //break;
    }
    server.clean();
    return 0;
}