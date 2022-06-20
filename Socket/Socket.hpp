/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 15:40:18 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/20 11:22:57 by atahiri          ###   ########.fr       */
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
#include <fcntl.h>

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
    std::vector<int> servers_fds;
    std::vector<struct sockaddr_in> vec_addresses;
    int address_len;

public:
    Socket(std::vector<ServerConfig> servers);
    ~Socket();
    // Socket(Socket const &rhs);
    // Socket &operator=(Socket const &rhs);
    std::vector<int> getServersFds();
    void _init(std::string host, int port);
    void _socket();
    void _bind();
    void _listen();
    void _accept();
    int acceptNewConnection(std::pair<int, size_t> pair);
    void _send(std::string msg);
    bool handleConnection(ServerConfig server_setup, int new_socket);
    void _recv();
    void _close();
};
