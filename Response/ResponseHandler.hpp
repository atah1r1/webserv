/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 14:35:35 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/26 16:13:54 by ehakam           ###   ########.fr       */
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
public:
	static std::pair<ServerConfig *, Location *> getConfig(Request req, std::vector<ServerConfig *> servers) {
		std::vector<ServerConfig *>::iterator it = servers.begin();

		// match Server & Location with same SERVERNAME & PATH
		while (it != servers.end())
		{
			if ((*it)->getServerName() == req.getHost()) {
				std::vector<Location *> _locations = (*it)->getLocations();
				Location* _l = matchLocation(_locations, req.getPath());
				if (_l != NULL) return std::make_pair(*it, _l);
			}
			++it;
		}

		// match Server & Location with EMPTY SERVERNAME & same PATH
		it = servers.begin();
		while (it != servers.end())
		{
			if ((*it)->getServerName().empty()) {
				std::vector<Location *> _locations = (*it)->getLocations();
				Location* _l = matchLocation(_locations, req.getPath());
				if (_l != NULL) return std::make_pair(*it, _l);
			}
			++it;
		}

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

	Response handleErrors(Request req, std::pair<ServerConfig *, Location *> config) {
		if (config.first == NULL && config.second == NULL) {
			// TODO: 404 Not found
		}
		return Response();
	}

	void handleCGIRequest(Request req, std::pair<ServerConfig *, Location *> config) {
		
	}
};

#endif // __RESPONSE_HANDLER_HPP__