/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:57:03 by aes-salm          #+#    #+#             */
/*   Updated: 2022/07/20 17:51:45 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(/* args */)
{
	_state = Request::FIRST_LINE;
	_method = "";
	_path = "";
	_version = "";
	_host = "";
	_port = 0;
	_headers = std::map<std::string, std::string>();
}

Request::~Request()
{
}

// GETTERS
std::string Request::getMethod()
{
	return _method;
}
std::string Request::getPath()
{
	return _path;
}
std::string Request::getVersion()
{
	return _version;
}
std::string Request::getHost()
{
	return _host;
}
int Request::getPort()
{
	return _port;
}
std::map<std::string, std::string> Request::getHeaders()
{
	return _headers;
}
Request::state Request::getState()
{
	return _state;
}

// SETTERS
void Request::setMethod(std::string method)
{
	_method = method;
}
void Request::setPath(std::string path)
{
	_path = path;
}
void Request::setVersion(std::string version)
{
	_version = version;
}
void Request::setHost(std::string host)
{
	_host = host;
}
void Request::setPort(int port)
{
	_port = port;
}
void Request::setState(Request::state state)
{
	_state = state;
}
void Request::setHeader(std::string key, std::string value)
{
	_headers[key] = value;
}