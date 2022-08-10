/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 15:40:18 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/10 11:43:45 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Config/ServerConfig.hpp"
#include "../Request/Request.hpp"
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
    bool opt;
    std::string hello;
    struct sockaddr_in address;
    //int port;
    std::string ip;
    std::vector<int> servers_fds;
    std::vector<struct sockaddr_in> vec_addresses;
    int address_len;
    std::map<int, Request> requests;

public:
    Socket(std::vector<ServerConfig> servers);
    ~Socket();
    // Socket(Socket const &rhs);
    // Socket &operator=(Socket const &rhs);
    std::vector<int> getServersFds();
    int acceptNewConnection(std::pair<int, size_t> pair);
    void _send(int my_socket, const char * msg, size_t length);
    bool handleConnection(ServerConfig server_setup, int new_socket);
    void _close();
    bool isThisRequestExist(int fd);
    void pushNewRequest(int fd);
    void removeRequest(int fd);
    Request receiveRequest(int fd);
};
