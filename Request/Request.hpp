/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:55:34 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/26 11:55:55 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Includes/Webserv.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/StatusCodes.hpp"

#define REQ_BUFFER_SIZE (2048)
class Request
{
public:
	Request(/* args */);
	~Request();
	Request(const Request &);
	Request &operator=(const Request &);

	// ENUM
	enum state
	{
		FIRST_LINE,
		HEADERS,
		BEFORE_BODY,
		UNCHUNKED_BODY,
		CHUNKED_BODY,
		COMPLETED
	};

	// GETTERS
	std::string getMethod() const;
	std::string getPath() const;
	std::string getVersion() const;
	std::string getHost() const;
	std::vector<std::pair<std::string, std::string> > getHeaders() const;
	std::string getHeader(std::string key) const;
	int getPort() const;
	Request::state getState() const;
	int getStatusCode() const;
	std::string getQueries() const;
	std::fstream &getBodyFile();
	std::string getBodyFileName() const;
	std::string getBodyTmp() const;
	int getBodyLength() const;
	bool isChunkSize() const;
	int getChunkSize() const;

	// SETTERS
	void setMethod(std::string method);
	void setPath(std::string path);
	void setVersion(std::string version);
	void setHost(std::string host);
	void setPort(int port);
	void setHeader(const std::string& key, const std::string& value);
	void setState(Request::state state);
	void setStatusCode(int statusCode);
	void setQueries(std::string queries);
	void setBodyFileName(std::string bodyFileName);
	void setBodyTmp(const std::string &tmp);
	void setBodyLength(int length);
	void setIsChunkSize(bool isChunkSize);
	void setChunkSize(int chunkSize);

	// METHODS
	void printRequest(void);
	int parseRequestError(std::string error, int statusCode);

private:
	Request::state _state;
	int _statusCode;
	std::string _method;
	std::string _path;
	std::string _queries;
	std::string _version;
	std::string _host;
	int _port;
	std::vector<std::pair<std::string, std::string> > _headers;
	std::fstream _body;
	std::string _bodyFileName;
	std::string _bodyTmp;
	int _bodyLength;
	bool _isChunkSize;
	int _chunkSize;
};

#endif