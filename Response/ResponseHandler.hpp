/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 14:35:35 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/26 19:57:34 by ehakam           ###   ########.fr       */
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

class ResponseHandler {
private:
	static Response _create501Error(Request req, std::pair<ServerConfig *, Location *> config) {
		// TODO: 501 Not Implemented
		return Response();
	}

	static Response _create400Error(Request req, std::pair<ServerConfig *, Location *> config) {
		// TODO: 400 Bad Request
		return Response();
	}

	static Response _create414Error(Request req, std::pair<ServerConfig *, Location *> config) {
		// TODO: 414 Request-URI Too Long
		return Response();
	}

	static Response _create413Error(Request req, std::pair<ServerConfig *, Location *> config) {
		// TODO: 413 Request Entity Too Large
		return Response();
	}

	static Response _create404Error(Request req, std::pair<ServerConfig *, Location *> config) {
		// TODO: 404 Not found
		return Response();
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
		if (req.getHeader("Transfer-Encoding") != "chunked") {
			Response r = _create501Error(req, config);
			return std::make_pair(true, r);
		}
		if (req.getHeader("Transfer-Encoding") == "" && req.getHeader("Content-Length") == "") {
			Response r = _create400Error(req, config);
			return std::make_pair(true, r);
		}
		if (req.getPath().length() > 2048) {
			Response r = _create414Error(req, config);
			return std::make_pair(true, r);
		}
		if (config.first != NULL && config.first->getClientBufferSize() > 0
			/*&& req.bodySize() > config.first->getClientBufferSize()*/) {
			Response r = _create413Error(req, config);
			return std::make_pair(true, r);
		}
		if (config.second == NULL) {
			Response r = _create404Error(req, config);
			return std::make_pair(true, r);
		}
		return std::make_pair(false, Response());
	}

	void handleCGIRequest(Request req, std::pair<ServerConfig *, Location *> config) {
		
	}
};

#endif // __RESPONSE_HANDLER_HPP__