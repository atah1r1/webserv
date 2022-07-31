/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:08:24 by atahiri           #+#    #+#             */
/*   Updated: 2022/07/31 16:05:02 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "Utils.hpp"

ServerConfig::ServerConfig(/* args */)
{
	std::vector<std::string> vec(1, "");
	setPort(0);
	this->_ip = "";
	setServerName("");
	setClientBufferSize(0);
	// setErrorPages(err_pages);
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
	this->_ip = ip_port[0];
	if (ip_port.size() > 1)
	{
		setPort(std::stoi(ip_port[1]));
	}
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

std::map<int, std::string> ServerConfig::getErrorPages() const
{
	return this->_error_pages;
}

void ServerConfig::setErrorPages(std::pair<int, std::string> _error_pages)
{
	this->_error_pages.insert(_error_pages);
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

void ServerConfig::setLocations(Location *_locations)
{
	this->_locations.push_back(_locations);
}

void ServerConfig::setIndexFile(std::vector<std::string> _index_file)
{
	this->_index_file = _index_file;
}

std::vector<std::string> ServerConfig::getIndexFiles(void)
{
	return this->_index_file;
}