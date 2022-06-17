/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 15:40:18 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/17 16:22:17 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Config/ServerConfig.hpp"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

class Socket
{
private:
    int server_fd;
    int new_socket;
    int valread;
    bool opt;
    char buffer[1024];
    std::string hello;
    struct sockaddr_in address;
    int port;
    std::string ip;
public:
    Socket();
    ~Socket();
    // Socket(Socket const &rhs);
    // Socket &operator=(Socket const &rhs);
    void _init();
    void _socket();
    void _bind();
    void _listen();
    void _accept();
    void _send(std::string msg);
    void _recv();
    void _close();
};
