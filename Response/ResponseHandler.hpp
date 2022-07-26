/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 14:35:35 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/27 00:41:31 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HANDLER_HPP__
#define __RESPONSE_HANDLER_HPP__

#include <iostream>
#include <utility>
#include <vector>
#include "Response.hpp"
#include "../Request/Request.hpp"
#include "../Config/ServerConfig.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/StatusCodes.hpp"

/*
<html>
<head><title>404 Not Found</title></head>
<body bgcolor="white">
<center><h1>404 Not Found</h1></center>
<hr><center>nginx/1.14.0 (Ubuntu)</center>
</body>
</html>
*/

class ResponseHandler {
private:
	static std::string _getStandardErrorBody(int statusCode) {
		std::stringstream ss;

		ss << "<html>" << "\n";
		ss << "<head><title>" << statusCode << " " << getReason(statusCode) << "</title></head>\n";
		ss << "<body bgcolor=\"white\">\n";
		ss << "<center><h1>" << statusCode << " " << getReason(statusCode) << "</h1></center>\n";
		ss << "<hr><center>" << SERVER_VERSION << "</center>";
		ss << "</body>\n";
		ss << "</html>\r\n";

		return ss.str();
	}

	static Response _createErrorPage(int statusCode) {
		Response r;
		std::string body = _getStandardErrorBody(statusCode);

		r.setVersion("HTTP/1.1");
		r.setStatusCode(statusCode);
		r.setStatus(getReason(statusCode));
		r.addHeader("Server", SERVER_VERSION);
		r.addHeader("Date", getCurrentDate());
		r.addHeader("Content-Type", "text/html");
		r.addHeader("Content-Length", toString<size_t>(body.length()));
		r.addHeader("Connection", "keep-alive");

		r.setBody(body);

		return r;
	}

public:
	static std::pair<ServerConfig *, Location *> getConfig(Request req, std::vector<ServerConfig *> servers) {
		std::vector<ServerConfig *>::iterator it = servers.begin();

		// match Server & Location with same SERVERNAME & PATH
		while (it != servers.end())
		{
			if ((*it)->getServerIp() == req.getHost() && (*it)->getPort() == req.getPort()) {
				std::vector<Location *> _locations = (*it)->getLocations();
				Location* _l = matchLocation(_locations, req.getPath());
				if (_l != NULL) return std::make_pair(*it, _l);
			}
			++it;
		}

		// match Server & Location with same SERVERNAME & PATH
		it = servers.begin();
		while (it != servers.end())
		{
			if ((*it)->getServerName() == req.getHost() && (*it)->getPort() == req.getPort()) {
				std::vector<Location *> _locations = (*it)->getLocations();
				Location* _l = matchLocation(_locations, req.getPath());
				if (_l != NULL) return std::make_pair(*it, _l);
			}
			++it;
		}

		// // match Server & Location with EMPTY SERVERNAME & same PATH
		// it = servers.begin();
		// while (it != servers.end())
		// {
		// 	if ((*it)->getServerName().empty() && (*it)->getPort() == req.getPort()) {
		// 		std::vector<Location *> _locations = (*it)->getLocations();
		// 		Location* _l = matchLocation(_locations, req.getPath());
		// 		if (_l != NULL) return std::make_pair(*it, _l);
		// 	}
		// 	++it;
		// }

		// // match Server with same SERVERNAME
		// it = servers.begin();
		// while (it != servers.end())
		// {
		// 	if ((*it)->getServerName() == req.getHost()) {
		// 		return std::make_pair(*it, (Location *)NULL);
		// 	}
		// 	++it;
		// }

		// // match Server with EMPTY SERVERNAME
		// it = servers.begin();
		// while (it != servers.end())
		// {
		// 	if ((*it)->getServerName().empty()) {
		// 		return std::make_pair(*it, (Location *)NULL);
		// 	}
		// 	++it;
		// }
		
		// no match has been found :(
		return std::make_pair((ServerConfig *)NULL, (Location *)NULL);
	}

	static std::pair<bool, Response> handleErrors(Request req, std::pair<ServerConfig *, Location *> config) {
		if (req.getHeader(H_TRANSFER_ENCODING) != "chunked") {
			Response r = _createErrorPage(NotImplemented); // 501
			return std::make_pair(true, r);
		}
		if (req.getHeader(H_TRANSFER_ENCODING) == "" && req.getHeader(H_CONTENT_LENGTH) == "") {
			Response r = _createErrorPage(BadRequest); // 400
			return std::make_pair(true, r);
		}
		if (req.getPath().length() > 2048) {
			Response r = _createErrorPage(URITooLong); // 414
			return std::make_pair(true, r);
		}
		if (config.first != NULL && config.first->getClientBufferSize() > 0
			/*&& req.bodySize() > config.first->getClientBufferSize()*/) {
			Response r = _createErrorPage(PayloadTooLarge); // 413
			return std::make_pair(true, r);
		}
		if (config.second == NULL) {
			Response r = _createErrorPage(NotFound); // 404
			return std::make_pair(true, r);
		}
		return std::make_pair(false, Response());
	}

	static Response handleCGIRequest(Request req, std::pair<ServerConfig *, Location *> config) {
		// check for request errors
		std::pair<bool, Response> _res = handleErrors(req, config);
		if (_res.first) return _res.second;

		// if no erros found
		// TODO: 
	}
};

#endif // __RESPONSE_HANDLER_HPP__