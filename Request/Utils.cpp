/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:22:25 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/17 23:48:43 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

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

int parseChunkedBody(Request *request)
{
	std::fstream &body = request->getBodyFile();
	if (!body.is_open())
		return request->parseRequestError("Body file is not open!!", 500);
	request->setBodyLength(request->getBodyLength() + request->getBodyTmp().length());
	body << request->getBodyTmp();
	request->setBodyTmp("");
	if (request->getHeader("Content-Length").compare("") && request->getBodyLength() >= toNumber<int>(request->getHeader("Content-Length")))
	{
		body << "\n";
		body.close();
		request->setState(Request::COMPLETED);
	}
	return 0;
}

int parseRequest(Request &request, std::string buffer)
{
	std::istringstream is(buffer);
	std::string line;
	int i = 0;

	// std::cout << buffer << std::endl;

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
		else if (request.getState() == Request::BEFORE_BODY)
		{
			request.setBodyFileName(randomFileName());
			std::cout << "[ logs ] : Setup request body file \"" << request.getBodyFileName() << "\"" << std::endl;
			std::fstream &bodyFile = request.getBodyFile();
			bodyFile.open(request.getBodyFileName(), std::fstream::in | std::fstream::out | std::fstream::trunc);

			// remove the headers part
			request.setBodyTmp(buffer.substr(buffer.find("\r\n\r\n") + 4, buffer.size()));

			if (request.getHeader("Transfer-Encoding") == "chunked")
				request.setState(Request::CHUNKED_BODY);
			else
				request.setState(Request::UNCHUNKED_BODY);
			break;
		}
		i++;
	}

	if (request.getState() == Request::UNCHUNKED_BODY)
		return parseChunkedBody(&request);
	else if (request.getState() == Request::CHUNKED_BODY)
	{
		// std::cout << "[ logs ] : Write chunked body to file" << std::endl;
		// std::fstream &bodyFile = request.getBodyFile();
		// bodyFile.write(request.getBodyTmp().c_str(), request.getBodyTmp().size());
		// bodyFile.close();
	}
	// request.printRequest();
	return 0;
}
