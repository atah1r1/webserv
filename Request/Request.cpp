/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:57:03 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/23 00:42:26 by ehakam           ###   ########.fr       */
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
	//_headers = std::map<std::string, std::string>();
	_bodyTmp = "";
	_bodyLength = 0;
	this->setHeader("Transfer-Encoding", "");
	this->setHeader("Content-Length", "");
}

Request::~Request()
{
}

Request::Request(const Request &other)
{
	*this = other;
}

Request &Request::operator=(const Request &other)
{
	if (this != &other)
	{
		_state = other._state;
		_statusCode = other._statusCode;
		_method = other._method;
		_path = other._path;
		_queries = other._queries;
		_version = other._version;
		_host = other._host;
		_port = other._port;
		_headers = other._headers;
	}
	return *this;
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
// std::map<std::string, std::string> Request::getHeaders() const
// {
// 	return _headers;
// }
std::vector<std::pair<std::string, std::string> > Request::getHeaders() const
{
	return _headers;
}
// std::string Request::getHeader(std::string key) const
// {
// 	std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
// 	if (it != this->_headers.end())
// 		return it->second;
// 	return "";
// }
std::string Request::getHeader(std::string key) const
{
	std::vector<std::pair<std::string, std::string> >::const_iterator it = this->_headers.begin();
	for (; it != this->_headers.end(); it++)
	{
		if (it->first == key)
			return it->second;
	}
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
std::fstream &Request::getBodyFile()
{
	return _body;
}
std::string Request::getBodyFileName() const
{
	return _bodyFileName;
}
std::string Request::getBodyTmp() const
{
	return _bodyTmp;
}
int Request::getBodyLength() const
{
	return _bodyLength;
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
void Request::setHeader(const std::string& key, const std::string& value)
{
	if (key == H_SET_COOKIE || key == H_HTTP_COOKIE)
	{
		_headers.push_back(std::make_pair(key, value));
		return;
	}
	std::vector<std::pair<std::string, std::string> >::iterator it = this->_headers.begin();
	for (; it != this->_headers.end(); it++)
	{
		if (it->first == key) {
			*it = std::make_pair(key, value);
			return;
		}
	}
	_headers.push_back(std::make_pair(key, value));
}
void Request::setStatusCode(int statusCode)
{
	_statusCode = statusCode;
}
void Request::setQueries(std::string queries)
{
	_queries = queries;
}
// void Request::setBodyFile(std::fstream &body)
// {
// 	_body = std::fstream(body);
// }
void Request::setBodyFileName(std::string bodyFileName)
{
	_bodyFileName = bodyFileName;
}
void Request::setBodyTmp(const std::string &tmp)
{
	_bodyTmp = tmp;
}
void Request::setBodyLength(int length)
{
	_bodyLength = length;
}

// METHODS
int Request::parseRequestError(std::string error, int statusCode)
{
	std::cout << "\033[1;31m[ ERROR ]: " << error << "\033[0m" << std::endl;
	this->setStatusCode(statusCode);
	this->setState(Request::COMPLETED);
	return -1;
}

void Request::printRequest(void)
{
	std::cout << "-------------- Print Request Object --------------" << std::endl;
	std::cout << "StatusCode: " << this->getStatusCode() << std::endl;
	std::cout << "Method: " << this->getMethod() << std::endl;
	std::cout << "Path: " << this->getPath() << std::endl;
	std::cout << "Queries: " << this->getQueries() << std::endl;
	std::cout << "Version: " << this->getVersion() << std::endl;
	std::cout << "Host: " << this->getHost() << std::endl;
	std::cout << "Port: " << this->getPort() << std::endl;
	std::cout << "ParseState: " << this->getState() << std::endl;
	std::cout << "BodyFileName: " << this->getBodyFileName() << std::endl;
	std::cout << "BodyLength: " << this->getBodyLength() << std::endl;
	std::cout << "Headers: " << std::endl;
	// std::map<std::string, std::string> headers = this->getHeaders();
	// std::map<std::string, std::string>::iterator it = headers.begin();
	// while (it != headers.end())
	// {
	// 	std::cout << it->first << ":" << it->second << std::endl;
	// 	it++;
	// }
	std::cout << "---------------------- End -----------------------" << std::endl;
}