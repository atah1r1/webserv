/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:55:34 by aes-salm          #+#    #+#             */
/*   Updated: 2022/07/26 19:34:00 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Includes/Webserv.hpp"

class Request
{
public:
	Request(/* args */);
	~Request();

	// ENUM
	enum state
	{
		FIRST_LINE = 0,
		BEFORE_HEADERS = 1,
		HEADERS = 2,
		BEFORE_BODY = 3,
		BODY = 4,
		COMPLETED = 5
	};

	// GETTERS
	std::string getMethod();
	std::string getPath();
	std::string getVersion();
	std::string getHost();
	std::map<std::string, std::string> getHeaders();
	std::string getHeader( std::string key );
	int getPort();
	Request::state getState();

	// SETTERS
	void setMethod(std::string method);
	void setPath(std::string path);
	void setVersion(std::string version);
	void setHost(std::string host);
	void setPort(int port);
	void setHeader(std::string key, std::string value);
	void setState(Request::state state);

private:
	Request::state _state;
	std::string _method;
	std::string _path;
	std::string _version;
	std::string _host;
	int _port;
	std::map<std::string, std::string> _headers;
};

#endif