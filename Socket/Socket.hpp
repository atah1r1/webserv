/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 12:21:06 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/27 11:16:23 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

#define QUEUE_SIZE 128

class Socket
{
private:
    int _sockfd;
    int _port;
    struct sockaddr_in _serv_addr;
    bool _isServSock;
    bool _keepAlive;
    std::string _host;
    int opt;
    bool _accepted;

public:
    Socket(bool isServ);
    Socket(const Socket &);
    Socket &operator=(const Socket &);
    ~Socket();

    bool operator==(const Socket &a);

    void launchSock();

    bool isServSock() const;
    bool keepAlive() const;
    void m_close() const;
    void setPort(int port);
    void setHost(std::string host);
    struct sockaddr_in getSockAddr();
    void setSockFd(int fd);
    void setSockAddr(struct sockaddr_in servAddr);
    int getSockFd() const;
    int getPort() const;
    void setServSock(bool serve);
    void updateConnection(bool connec);
    bool isAccepted() const;
    void setAccepted( bool accepted );
};

#endif