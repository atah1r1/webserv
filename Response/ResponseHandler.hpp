/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 14:35:35 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/30 19:18:31 by ehakam           ###   ########.fr       */
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
#include "../Utils/FileHandler.hpp"

class ResponseHandler {
private:
	static std::string _getDefaultErrorBody(int statusCode, std::pair<ServerConfig *, Location *> config) {
		// TODO: find default error_page path (if exists) by statusCode
		// if no error_page exists in config, or failed to open error_page path, return standardErrorBody
		return _getStandardErrorBody(statusCode);
	}

	static std::string _getStandardErrorBody(int statusCode) {
		std::stringstream ss;

		ss << "<html>" << "\n";
		ss << "<head><title>" << statusCode << " " << getReason(statusCode) << "</title></head>\n";
		ss << "<body bgcolor=\"white\">\n";
		ss << "<center><h1>" << statusCode << " " << getReason(statusCode) << "</h1></center>\n";
		ss << "<hr><center>" << SERVER_VERSION << "</center>";
		ss << "</body>\n";
		ss << "</html>\n";

		return ss.str();
	}

	static std::string _getDirListingBody( const std::string& uri, const std::string& root, const std::string& dirPath ) {
		std::stringstream ss;

		std::string _dirPath = FileHandler::disconnectPath(root, dirPath);
		std::map<std::string, std::string> _paths = FileHandler::exploreDir(root, dirPath);

		ss << "<html>\n";
		ss << "<head><title>" << "Directory listing for " << _dirPath << "</title></head>\n";
		ss << "<body bgcolor=\"white\">\n";
		ss << "<h1>" << "Directory listing for " << _dirPath << "</h1>\n";
		ss << "<hr>\n<ul>\n";

		std::map<std::string, std::string>::iterator it = _paths.begin();
		while (it != _paths.end())
			ss << "<li><a href=\"" << uri << it->second << "\">" << it->first << "</a></li>\n";

		ss << "</ul>\n<hr>\n";
		ss << "</body>\n";
		ss << "</html>\n";

		return ss.str();
	}

	static Response _createErrorResponse(int statusCode, std::pair<ServerConfig *, Location *> config) {
		Response r;
		std::string body = _getDefaultErrorBody(statusCode, config);

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

	static Response _createDirListingResponse( const std::string& uri, const std::string& root, const std::string& dirPath ) {
		Response r;
		std::string body = _getDirListingBody(uri, root, dirPath);

		r.setVersion("HTTP/1.1");
		r.setStatusCode(OK);
		r.setStatus(getReason(OK));
		r.addHeader("Server", SERVER_VERSION);
		r.addHeader("Date", getCurrentDate());
		r.addHeader("Content-Type", "text/html");
		r.addHeader("Content-Length", toString<size_t>(body.length()));
		r.addHeader("Connection", "keep-alive");

		r.setBody(body);

		return r;
	}

	static Response _createRedirectionResponse( int statusCode, const std::string& dirPath ) {
		Response r;

		r.setVersion("HTTP/1.1");
		r.setStatusCode(statusCode);
		r.setStatus(getReason(statusCode));
		r.addHeader("Server", SERVER_VERSION);
		r.addHeader(H_LOCATION, dirPath);

		return r;
	}

public:
	static std::pair<ServerConfig *, Location *> getMatchingConfig(Request req, std::vector<ServerConfig *> servers) {
		std::vector<ServerConfig *>::iterator it = servers.begin();

		// match Server & Location with same HOST & PATH
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

	static Response handleRequests( Request req, std::pair<ServerConfig *, Location *> config ) {
		// check for request errors
		std::pair<bool, Response> _res = handleRequestErrors(req, config);
		if (_res.first) return _res.second;

		// no errors / redirections
		std::string _m = toUpperCase(trim(req.getMethod()));

		if (_m == GET) {
			// TODO: handle GET
		} else if (_m == POST) {
			// TODO: handle POST
		} else if (_m == DELETE) {
			// TODO: handle DELETE
		} else {
			// normally it shouldn't get here but if so:
			return _createErrorResponse(MethodNotAllowed, config); 
		}
	}

	static std::pair<bool, Response> handleRequestErrors(Request req, std::pair<ServerConfig *, Location *> config) {
		if (req.getHeader(H_TRANSFER_ENCODING) != "chunked") {
			Response r = _createErrorResponse(NotImplemented, config); // 501
			return std::make_pair(true, r);
		}
		if (req.getHeader(H_TRANSFER_ENCODING) == "" && req.getHeader(H_CONTENT_LENGTH) == "") {
			Response r = _createErrorResponse(BadRequest, config); // 400
			return std::make_pair(true, r);
		}
		if (req.getPath().length() > 2048) {
			Response r = _createErrorResponse(URITooLong, config); // 414
			return std::make_pair(true, r);
		}
		if (config.first != NULL && config.first->getClientBufferSize() > 0
			/*&& req.bodySize() > config.first->getClientBufferSize()*/) {
			Response r = _createErrorResponse(PayloadTooLarge, config); // 413
			return std::make_pair(true, r);
		}
		if (config.second == NULL) {
			Response r = _createErrorResponse(NotFound, config); // 404
			return std::make_pair(true, r);
		}
		if (!config.second->_redirection_path.empty()) {
			// check if redir path start with /, else add it
			if (config.second->_redirection_path.front() != '/')
				config.second->_redirection_path.insert(config.second->_redirection_path.begin(), '/');
			Response r = _createRedirectionResponse(MovedPermanently, config.second->_redirection_path);
			return std::make_pair(true, r);
		}
		if (!config.second->_allow_methods.empty() && !isMethodAllowed(config.second->_allow_methods, req.getMethod())) {
			Response r = _createErrorResponse(MethodNotAllowed, config); // 405
			return std::make_pair(true, r);
		}
		if (!isMethodImplemented(req.getMethod())) {
			Response r = _createErrorResponse(NotImplemented, config); // 501
			return std::make_pair(true, r);
		}
		return std::make_pair(false, Response());
	}

	static Response handleCGIRequest(Request req, std::pair<ServerConfig *, Location *> config) {
		// check for request errors
		std::pair<bool, Response> _res = handleRequestErrors(req, config);
		if (_res.first) return _res.second;

		// if no erros found
		// TODO: 
	}

	static Response handleGETRequest( Request req, std::pair<ServerConfig *, Location *> config ) {
		// check for request errors
		std::pair<bool, Response> _res = handleRequestErrors(req, config);
		if (_res.first) return _res.second;

		ServerConfig* _conf = config.first;
		Location* _loc = config.second;

		std::string _requestPath = FileHandler::getFullPath(_loc->_root, req.getPath());
		// check if path exists and is readable
		if (FileHandler::pathExists(_requestPath)) {
			// path exists
			if (FileHandler::isPathReadable(_requestPath)) {
				// path readable
				FileType _type = FileHandler::getType(_requestPath);
				// check path type
				if (_type == T_DIR) {
					// path = directory
					// check redir
					if (_requestPath.back() != '/') {
						return _createRedirectionResponse(MovedPermanently, _requestPath + "/");
					} else {
						// check for index files
						std::string _indexPath = FileHandler::searchIndexes(_requestPath, _loc->_index_file);
						if (!_indexPath.empty()) {
							// index file exists
							// TODO: serve index file => CGI/file
						} else {
							// index file doesn't exist
							// check for autoindex
							if (_loc->_autoindex) {
								// autoindex on
								
							} else {
								// autoindex off
								return _createErrorResponse(Forbidden, config);
							}
						}
					}
				} else if (_type == T_FILE) {
					// path = file
					// TODO: serve file => CGI/file
				} else {
					// path = other
					return _createErrorResponse(InternalServerError, config);
				}
			} else {
				// path not readable
				return _createErrorResponse(Forbidden, config);
			}
		} else {
			// path does not exist
			return _createErrorResponse(NotFound, config);
		}

		// path is dir but does not end with "/" 
		

		// 
		
	}

};

#endif // __RESPONSE_HANDLER_HPP__