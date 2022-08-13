/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 12:24:52 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/12 03:09:54 by ehakam           ###   ########.fr       */
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

void Server::performSelect()
{
    fd_set tmpReadSet = this->_readSet;
    fd_set tmpWriteSet = this->_writeSet;

    // wait for events in ReadSet and WriteSet
    int result = select(this->_maxFd + 1, &tmpReadSet, &tmpWriteSet, NULL, NULL);
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

void Server::acceptNewClient(Socket *sock)
{
    // accept connection on server socket and get fd for new Client
    int newClient = accept(sock->getSockFd(), 0, 0);
    Socket *client = new Socket(false);
    client->setSockFd(newClient);

    client->setSockAddr(sock->getSockAddr());
    this->_clients.insert(std::make_pair(newClient, Request()));

    this->_sockets.push_back(client);

    // add our client to read set
    std::cout << "New client connected: " << client->getSockFd() << std::endl;
    addToSet(client->getSockFd(), this->_readSet);
}

// void reset(int newClient)
// {
//     this->_clients.insert(std::make_pair(newClient, Request()));
// }

void Server::handleClient(Socket *client)
{
    char buff[MAX_BUFFER_SIZE];

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
        return;
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
        }
    }
}

void Server::sendResponse(Socket *client)
{
    Response r = ResponseHandler::handleRequests(this->_clients[client->getSockFd()], this->_servConf.front());
    // send(client->getSockFd(), hello.c_str(), strlen(hello.c_str()), 0);
    std::string response = r.toString();
    // send Headers to client
    int size = send(client->getSockFd(), response.c_str(), response.size(), 0);
    // send chunked body response chunked
    char buffer[BUFFER_SIZE] = {0};
    size_t len = 0;
    if (r.isBuffered())
    {
        size_t s = 0;
        while ((len = r.getNextBuffer(buffer)) > 0)
        {
            send(client->getSockFd(), buffer, len, 0);
            s += len;
        }
        std::cout << "Sent chunk " << s << " of size " << len << std::endl;
    }
    if (size == -1)
    {
        std::cout << "ENTERED" << std::endl;
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
    if (this->_clients[client->getSockFd()].getHeader("Connection") == "keep-alive")
    {
        this->_clients.insert(std::make_pair(client->getSockFd(), Request()));
        deleteFromSet(client->getSockFd(), this->_writeSet);
        addToSet(client->getSockFd(), this->_readSet);
    }
    else
    {
        deleteFromSet(client->getSockFd(), this->_writeSet);
        deleteFromSet(client->getSockFd(), this->_readSet);
        this->_clients.erase(client->getSockFd());
        client->m_close();
        std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), client);
        if (position != this->_sockets.end())
        {
            delete (*position);
            this->_sockets.erase(position);
        }
    }
    // deleteFromSet(client->getSockFd(), this->_writeSet);
    // this->_clients.erase(client->getSockFd());
    // client->m_close();
    // std::vector<Socket *>::iterator position = std::find(this->_sockets.begin(), this->_sockets.end(), client);
    // if (position != this->_sockets.end())
    // {
    //     delete (*position);
    //     this->_sockets.erase(position);
    // }
}

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
    while (1)
    {
        // FIX: Broken pipe signal handler
        signal(SIGPIPE, SIG_IGN);
        server.performSelect();
    }
    server.clean();
    return 0;
}