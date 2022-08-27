/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:22:25 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/27 12:23:55 by atahiri          ###   ########.fr       */
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
	request->setBodyLength(request->getBodyLength() + request->getBodyTmp().size());
	body << request->getBodyTmp();
	if (request->getBodyLength() == toNumber<int>(request->getHeader("Content-Length")))
	{
		body.close();
		request->setState(Request::COMPLETED);
	}
	request->setBodyTmp("");
	return 0;
}

int unChunkFile(Request *request) {
	std::fstream newBody;
	std::ifstream body;
	body.open(request->getBodyFileName(), std::fstream::binary);
	newBody.open(request->getBodyFileName() + "_unchunked", std::fstream::binary | std::fstream::out | std::fstream::trunc);

	if (!body.is_open() || !newBody.is_open())
		return request->parseRequestError("Body file is not open!!", 500);

	std::string line;
	while (std::getline(body, line))
	{
		request->setChunkSize(hexToDecimal(trim(line)));
		if (request->getChunkSize() == 0)
			break;
		std::vector<char> buffer(request->getChunkSize());
		body.read(buffer.data(), request->getChunkSize());
		newBody.write(buffer.data(), request->getChunkSize());
		body.ignore(2); // ignore \r
	}
	body.close();
	newBody.close();
	std::remove(request->getBodyFileName().c_str());
	request->setBodyFileName(request->getBodyFileName() + "_unchunked");
	request->setState(Request::COMPLETED);
	return 0;
}

int parseChunkedBody(Request *request)
{
	std::fstream &body = request->getBodyFile();
	
	if (!body.is_open())
		return request->parseRequestError("Body file is not open!!", 500);
	body << request->getBodyTmp();
	if (request->getBodyTmp().find("0\r\n\r\n") != std::string::npos)
	{
		body.close();
		request->setBodyTmp("");
		return unChunkFile(request);
	}
	return 0;
}

int parseRequest(Request &request, char *buffer, size_t size)
{
	std::istringstream is(buffer, size);
	std::string tmp(buffer, size);
	size_t headers_size = 0;
	std::string line;
	int i = 0;

	if (request.getState() == Request::CHUNKED_BODY || request.getState() == Request::UNCHUNKED_BODY)
		request.setBodyTmp(tmp);
	else
	{
		while (std::getline(is, line))
		{
			headers_size += line.size() + 1;

			if (i == 0 && request.getState() == Request::FIRST_LINE) {
				parseFirstLine(line, &request);
				continue;
			}

			if (request.getState() == Request::HEADERS)
			{
				if (line == "\r")
				{
					request.setState(Request::BEFORE_BODY);
					if (request.getMethod() != "POST")
					{
						request.setState(Request::COMPLETED);
						break;
					}
				}
				else
					parseHeaders(line, &request);
			}

			if (request.getState() == Request::BEFORE_BODY)
			{
				// check if there is no Content-Length or Transfer-Encoding
				if (request.getHeader("Content-Length") == "" && request.getHeader("Transfer-Encoding") != "chunked")
					return request.parseRequestError("No Content-Length", 400);
				if (request.getHeader("Transfer-Encoding") != "chunked" && request.getHeader("Transfer-Encoding") != "")
					return request.parseRequestError("Invalid Transfer-Encoding", 400);

				request.setBodyFileName(randomFileName());
				std::fstream &bodyFile = request.getBodyFile();
				bodyFile.open(request.getBodyFileName(), std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);

				request.setBodyTmp(std::string(tmp.begin() + headers_size, tmp.end())); // remove headers part from request

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
		return parseChunkedBody(&request);

	return 0;
}
