/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:55:34 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/05 11:47:09 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Includes/Webserv.hpp"
#include "../Utils/Utils.hpp"

class Request
{
public:
	Request(/* args */);
	~Request();

	// ENUM
	enum state
	{
		FIRST_LINE = 0,
		HEADERS = 1,
		BEFORE_BODY = 2,
		BODY = 3,
		COMPLETED = 4
	};

	// GETTERS
	std::string getMethod() const;
	std::string getPath() const;
	std::string getVersion() const;
	std::string getHost() const;
	std::map<std::string, std::string> getHeaders() const;
	std::string getHeader(std::string key) const;
	int getPort() const;
	Request::state getState() const;
	int getStatusCode() const;
	std::string getQueries() const;

	// SETTERS
	void setMethod(std::string method);
	void setPath(std::string path);
	void setVersion(std::string version);
	void setHost(std::string host);
	void setPort(int port);
	void setHeader(std::string key, std::string value);
	void setState(Request::state state);
	void setStatusCode(int statusCode);
	void setQueries(std::string queries);

private:
	Request::state _state;
	int _statusCode;
	std::string _method;
	std::string _path;
	std::string _queries;
	std::string _version;
	std::string _host;
	int _port;
	std::map<std::string, std::string>
		_headers;
};

#endif