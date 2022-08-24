/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:22:25 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/24 23:30:37 by ehakam           ###   ########.fr       */
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
	int port = 80;
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

int parseUnchunkedBody(Request *request)
{
	std::fstream &body = request->getBodyFile();

	if (!body.is_open())
		return request->parseRequestError("Body file is not open!!", 500);
	request->setBodyLength(request->getBodyLength() + request->getBodyTmp().length());
	body << request->getBodyTmp();
	request->setBodyTmp("");
	if (request->getBodyLength() == toNumber<int>(request->getHeader("Content-Length")))
	{
		body.close();
		request->setState(Request::COMPLETED);
	}
	return 0;
}

// int parseChunkedBody(Request *request)
// {
// 	std::fstream &body = request->getBodyFile();
// 	std::istringstream iss(request->getBodyTmp());
// 	std::string line;
// 	std::string token;
// 	int chunkSize;
// 	int i = 0;

// 	if (!body.is_open())
// 		return request->parseRequestError("Body file is not open!!", 500);
	
		
// 	while (std::getline(iss, line))
// 	{
		
// 	}
// 	request->setBodyTmp("");
// 	return 0;
// }

int parseRequest(Request &request, char *buffer, size_t size)
{
	std::istringstream is(buffer, size);
	std::string tmp(buffer, size);
	std::string line;
	int i = 0;

	if (request.getState() == Request::CHUNKED_BODY || request.getState() == Request::UNCHUNKED_BODY)
		request.setBodyTmp(tmp);
	else
	{
		while (std::getline(is, line))
		{
			if (i == 0 && request.getState() == Request::FIRST_LINE)
				parseFirstLine(line, &request);
			else if (request.getState() == Request::HEADERS)
			{
				if (line == "\r")
				{
					request.setState(Request::BEFORE_BODY);
					if (request.getMethod() != "POST")
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
				std::fstream &bodyFile = request.getBodyFile();
				bodyFile.open(request.getBodyFileName(), std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);

				// remove the headers part
				request.setBodyTmp(tmp.substr(tmp.find("\r\n\r\n") + 4, tmp.size()));

				if (request.getHeader(H_TRANSFER_ENCODING) == "chunked")
					request.setState(Request::CHUNKED_BODY);
				else
					request.setState(Request::UNCHUNKED_BODY);
				break;
			}
			i++;
		}
	}

	if (request.getState() == Request::UNCHUNKED_BODY)
		return parseUnchunkedBody(&request);
	else if (request.getState() == Request::CHUNKED_BODY)
		throw std::runtime_error("Not implemented yet");

	request.printRequest();
	return 0;
}
