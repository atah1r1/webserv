/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:22:25 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/26 12:38:08 by aes-salm         ###   ########.fr       */
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
	
	// std::cout << "Body length: " << request->getBodyLength() << std::endl;
	std::cout << "Content-Length: |" << request->getHeader("Content-Length")<< "|" << std::endl;

	request->setBodyTmp("");
	if (request->getBodyLength() == toNumber<int>(request->getHeader("Content-Length")))
	{
		body.close();
		request->setState(Request::COMPLETED);
	}
	return 0;
}

int parseChunkedBody(Request *request)
{
	std::fstream &body = request->getBodyFile();
	std::string chunkData = request->getBodyTmp();
	request->setBodyTmp("");
	size_t endLine;

	while ((endLine = chunkData.find("\r\n")) != std::string::npos)
	{
		if (request->isChunkSize()) {
			std::string chunkSizeHexa = chunkData.substr(0, endLine); // get chunk size ex: 5\r\n
			request->setChunkSize(hexToDecimal(chunkSizeHexa));
			std::cout << "Chunk size: " << request->getChunkSize() << std::endl;
			chunkData.erase(0, endLine + 2); // remove chunk size and \r\n
			request->setIsChunkSize(false);
		} else {
			if (request->getChunkSize() == 0) {
				body.close();
				request->setState(Request::COMPLETED);
				return 0;
			}
			if (!body.is_open())
				return request->parseRequestError("Body file is not open!!", 500);
			request->setBodyLength(request->getBodyLength() + request->getChunkSize());
			body << chunkData.substr(0, endLine); // write chunk data
			chunkData.erase(0, endLine + 2); // remove chunk data and \r\n
			request->setIsChunkSize(true);
			request->setChunkSize(0);
		}
	}
	return 0;
}

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
				// check if there is no Content-Length or Transfer-Encoding
				if (request.getHeader("Content-Length") == "")
					return request.parseRequestError("No Content-Length", 400);
				if (request.getHeader("Transfer-Encoding") != "chunked" && request.getHeader("Transfer-Encoding") != "")
					return request.parseRequestError("Invalid Transfer-Encoding", 400);

				request.setBodyFileName(randomFileName());
				std::fstream &bodyFile = request.getBodyFile();
				bodyFile.open(request.getBodyFileName(), std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);

				// remove the headers part
				request.setBodyTmp(tmp.substr(tmp.find("\r\n\r\n") + 4, tmp.size()));

				std::cout << "---------- Chunked Body ----------" << std::endl;
				std::cout<< request.getBodyTmp() << std::endl;
				std::cout << "---------- Chunked Body ----------" << std::endl;

				if (request.getHeader(H_TRANSFER_ENCODING) == "chunked")
				{
					request.setIsChunkSize(true);
					request.setState(Request::CHUNKED_BODY);
				}
				else
					request.setState(Request::UNCHUNKED_BODY);
				break;
			}
			i++;
		}
	}

	// request.printRequest();
	
	if (request.getState() == Request::UNCHUNKED_BODY)
		return parseUnchunkedBody(&request);
	else if (request.getState() == Request::CHUNKED_BODY)
		return parseChunkedBody(&request);

	return 0;
}
