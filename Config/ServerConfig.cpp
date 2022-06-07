/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:08:24 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/07 20:16:17 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "Utils.hpp"

ServerConfig::ServerConfig(/* args */)
{
    // setPort(80);
    // setServerName("www");
    // setRoot("./www/");

    // std::vector<std::string> error_pages;
    // error_pages.push_back("500");
    // error_pages.push_back("502");
    // error_pages.push_back("503");
    // error_pages.push_back("504");
    // error_pages.push_back("/custom_50x.html");
    // setErrorPages(error_pages);

    // std::vector<std::string> allow_methods;
    // allow_methods.push_back("GET");
    // allow_methods.push_back("POST");
    // allow_methods.push_back("DELETE");
    // setAllowMethods(allow_methods);

    // setClientBufferSize(100);
    // setAutoIndex(true);

    // std::vector<Location *> locations;
    // Location *location = new Location();
    // location->_location = "/dir";
    // location->_autoindex = false;
    // location->_index_file = "index.html";
    // location->_client_buffer_size = "100";
    // location->_allow_methods.push_back("GET");
    // location->_allow_methods.push_back("POST");
    // locations.push_back(location);

    // setLocations(locations);
}

ServerConfig::~ServerConfig()
{
}

std::string ServerConfig::getServerName(void) const
{
    return this->_name;
}

void ServerConfig::setServerName(std::string _name)
{
    this->_name = _name;
}

std::string ServerConfig::getServerIp(void) const
{
    return this->_ip;
}

void ServerConfig::setServerIp(std::string _ip)
{
    std::vector<std::string> ip_port = split(_ip, ":");
    _ip = ip_port[0];
    setPort(std::stoi(ip_port[1]));
}

int ServerConfig::getPort(void) const
{
    return this->_port;
}

void ServerConfig::setPort(int _port)
{
    this->_port = _port;
}

std::string ServerConfig::getRoot(void) const
{
    return this->_root;
}

void ServerConfig::setRoot(std::string _root)
{
    this->_root = _root;
}

std::vector<std::string> ServerConfig::getErrorPages() const
{
    return this->_error_pages;
}

void ServerConfig::setErrorPages(std::vector<std::string> _error_pages)
{
    this->_error_pages = _error_pages;
}

std::vector<std::string> ServerConfig::getAllowMethods() const
{
    return this->_allow_methods;
}

void ServerConfig::setAllowMethods(std::vector<std::string> _allow_methods)
{
    this->_allow_methods = _allow_methods;
}

int ServerConfig::getClientBufferSize() const
{
    return this->_client_buffer_size;
}

void ServerConfig::setClientBufferSize(int _client_buffer_size)
{
    this->_client_buffer_size = _client_buffer_size;
}

bool ServerConfig::getAutoIndex() const
{
    return this->_autoindex;
}

void ServerConfig::setAutoIndex(bool _autoindex)
{
    this->_autoindex = _autoindex;
}

std::vector<Location *> ServerConfig::getLocations()
{
    return this->_locations;
}

void ServerConfig::setLocations(t_location _locations)
{
    // this->_locations.push_back(_locations);
}

void ServerConfig::setIndexFile(std::vector<std::string> _index_file)
{
    this->_index_file = _index_file;
}