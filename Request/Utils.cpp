/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:22:25 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/07 22:17:16 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void printRequest(Request request)
{
	std::cout << "-------------- Print Request Object --------------" << std::endl;
	std::cout << "StatusCode: " << request.getStatusCode() << std::endl;
	std::cout << "Method: " << request.getMethod() << std::endl;
	std::cout << "Path: " << request.getPath() << std::endl;
	std::cout << "Queries: " << request.getQueries() << std::endl;
	std::cout << "Version: " << request.getVersion() << std::endl;
	std::cout << "Host: " << request.getHost() << std::endl;
	std::cout << "Port: " << request.getPort() << std::endl;
	std::cout << "ParseState: " << request.getState() << std::endl;
	std::cout << "Headers: " << std::endl;
	std::map<std::string, std::string> headers = request.getHeaders();
	std::map<std::string, std::string>::iterator it = headers.begin();
	while (it != headers.end())
	{
		std::cout << it->first << ":" << it->second << std::endl;
		it++;
	}
	std::cout << "---------------------- End -----------------------" << std::endl;
}

void parseFirstLine(std::string line, Request *request)
{
	std::istringstream iss(line);
	std::string token;
	int i = 0;
	while (std::getline(iss, token, ' '))
	{
		if (i == 0)
			request->setMethod(token);
		else if (i == 1)
		{
			try
			{
				request->setQueries(token.substr(token.find("?"), token.size()));
				request->setPath(token.substr(0, token.find("?")));
			}
			catch (const std::exception &e)
			{
				request->setPath(token);
			}
		}
		else if (i == 2)
			request->setVersion(token);
		else
			break;
		i++;
	}
	request->setState(Request::HEADERS);
}

void parseHeaders(std::string line, Request *request)
{
	std::istringstream iss(line);
	std::string token;
	std::string key;
	std::string value;
	int port;
	int j = 0;
	while (std::getline(iss, token, ':'))
	{
		if (j == 0)
			key = toHeaderCase(token);
		else if (j == 1)
			value = trim(token); // remove first char ' '
		else if (j == 2)
			port = toNumber<int>(token);
		j++;
	}
	if (key == "Host")
	{
		request->setHost(value);
		request->setPort(port);
	}
	else
		request->setHeader(key, value);
}

void parseBody(std::string line, Request *request)
{
	if (request->getState() == Request::BEFORE_BODY)
		request->setState(Request::BODY);

	std::cout << "Body: " << line << std::endl;
}

void parseRequest(Request request, std::string buffer)
{
	std::istringstream is(buffer);
	std::string line;
	int i = 0;

	while (std::getline(is, line))
	{
		if (i == 0 && request.getState() == Request::FIRST_LINE)
			parseFirstLine(line, &request);
		else if (request.getState() == Request::HEADERS)
		{
			if (line == "\r")
			{
				request.setState(Request::BEFORE_BODY);
				if (request.getMethod() == "GET" || request.getMethod() == "DELETE")
				{
					request.setState(Request::COMPLETED);
					break;
				}
				continue;
			}
			parseHeaders(line, &request);
		}
		else if (request.getState() == Request::BEFORE_BODY || request.getState() == Request::BODY)
			parseBody(line, &request);
		i++;
	}
	// printRequest(request);
}
