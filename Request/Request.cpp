/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:57:03 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/02 11:29:41 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(/* args */)
{
	_state = Request::FIRST_LINE;
	_statusCode = 0;
	_method = "";
	_path = "";
	_queries = "";
	_version = "";
	_host = "";
	_port = 0;
	_headers = std::map<std::string, std::string>();
}

Request::~Request()
{
}

// GETTERS
std::string Request::getMethod() const
{
	return _method;
}
std::string Request::getPath() const
{
	return _path;
}
std::string Request::getVersion() const
{
	return _version;
}
std::string Request::getHost() const
{
	return _host;
}
int Request::getPort() const
{
	return _port;
}
std::map<std::string, std::string> Request::getHeaders() const
{
	return _headers;
}
std::string Request::getHeader(std::string key) const
{
	std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
	if (it != this->_headers.end())
		return it->second;
	return "";
}
Request::state Request::getState() const
{
	return _state;
}
int Request::getStatusCode() const
{
	return _statusCode;
}
std::string Request::getQueries() const
{
	return _queries;
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
void Request::setStatusCode(int statusCode)
{
	_statusCode = statusCode;
}
void Request::setQueries(std::string queries)
{
	_queries = queries;
}