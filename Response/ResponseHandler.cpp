/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 22:24:39 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/02 00:56:56 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHandler.hpp"

std::string ResponseHandler::_getDefaultErrorBody( int statusCode, std::pair<ServerConfig *, Location *> config ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;

	std::string _errorPage = _conf->getErrorPage(statusCode);

	if (!_errorPage.empty()) {
		std::string _pagePath = FileHandler::getFullPath(_loc->_root, _errorPage);
		if (FileHandler::isPathReadable(_pagePath))
			return FileHandler::readFile(_pagePath);
	}

	return _getStandardErrorBody(statusCode);
}

std::string ResponseHandler::_getStandardErrorBody( int statusCode ) {
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

std::string ResponseHandler::_getDirListingBody( const std::string& uri, const std::string& root, const std::string& dirPath ) {
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

Response ResponseHandler::_createErrorResponse( int statusCode, std::pair<ServerConfig *, Location *> config ) {
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

Response ResponseHandler::_createDirListingResponse( const std::string& uri, const std::string& root, const std::string& dirPath ) {
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

Response ResponseHandler::_createRedirectionResponse( int statusCode, const std::string& dirPath ) {
	Response r;

	r.setVersion("HTTP/1.1");
	r.setStatusCode(statusCode);
	r.setStatus(getReason(statusCode));
	r.addHeader("Server", SERVER_VERSION);
	r.addHeader(H_LOCATION, dirPath);

	return r;
}

Response ResponseHandler::_createFileResponse( const std::string& filePath ) {
	Response r;
	std::string body = FileHandler::readFile(filePath);

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

Response ResponseHandler::_createFileCGIResponse( Request req, ServerConfig *conf, Location * loc, const std::string& filePath ) {
	// char buff[1024] = {0};

	// std::vector<const char *> v;
	// v.push_back(strdup((std::string("REQUEST_METHOD") + "=" + req.getMethod()).c_str()));
	// v.push_back(strdup((std::string("PATH") + "=" + (getenv("PATH") ?: "")).c_str()));
	// v.push_back(strdup((std::string("AUTH_TYPE") + "=null").c_str()));
	// v.push_back(strdup((std::string("TERM") + "=" + (getenv("TERM") ?: "")).c_str()));
	// v.push_back(strdup((std::string("HOME") + "=" + (getenv("HOME") ?: "")).c_str()));
	// gethostname(buff, 100);
	// v.push_back(strdup((std::string("HOSTNAME") + "=" + buff).c_str()));
	// getlogin_r(buff, 100);
	// v.push_back(strdup((std::string("USER") + "=" + buff).c_str()));

	// size_t n = req.getPath().find_first_of('?');
	// n = n == std::string::npos ? req.getPath().length() : n + 1;
	// v.push_back(strdup((std::string("CONTENT_LENGTH") + "=" + req.getHeader(H_CONTENT_LENGTH)).c_str()));

	// v.push_back(strdup((std::string("CONTENT_TYPE") + "=" + req.getHeader(H_CONTENT_TYPE)).c_str()));
	// v.push_back(strdup((std::string("GATEWAY_INTERFACE") + "=CGI/1.1").c_str()));

	// v.push_back(strdup((std::string("PATH_INFO") + "=" + req.getPath().substr(0, n)).c_str()));
	// v.push_back(strdup((std::string("PATH_TRANSLATED") + "=" + _indexPath).c_str()));
	// v.push_back(strdup((std::string("REMOTE_ADDR") + "=" + _conf->getServerIp()).c_str()));
	// v.push_back(strdup((std::string("REMOTE_HOST") + "=" + req.getHeader("host").substr(0, req.getHeader("host").find_first_of(':'))).c_str()));

	// v.push_back(strdup((std::string("SERVER_NAME") + "=" + _conf->getServerName()).c_str()));
	// v.push_back(strdup((std::string("SERVER_PORT") + "=" + toString<int>(_conf->getPort())).c_str()));
	// v.push_back(strdup((std::string("SERVER_PROTOCOL") + "=HTTP/1.1").c_str()));
	// v.push_back(strdup((std::string("SERVER_SOFTWARE") + "=" + SERVER_VERSION).c_str()));

	// v.push_back(strdup((std::string("QUERY_STRING") + "=" + req.getPath().substr(n)).c_str()));
	// v.push_back(strdup((std::string("HTTP_COOKIE") + "=" + req.getHeader("Cookie")).c_str()));
	// v.push_back(strdup((std::string("REDIRECT_STATUS") + "=").c_str()));
	// v.push_back(NULL);

	//std::string _cgiResponse = cgi(_indexPath, const_cast<char *const*>(v.data()));
	//Response r = Response::parseFrom(_cgiResponse);
	//r.addHeader("Server", SERVER_VERSION);

	Response r;
	std::string body = "CGI STUFF YAAAY" CRLF;

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

Response ResponseHandler::handleGETFile( Request req, std::pair<ServerConfig *, Location *> config, const std::string& requestPath  ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;

	// cgi
	if (FileHandler::requiresCGI(requestPath)) {
		return _createFileCGIResponse(req, _conf, _loc, requestPath);
	}

	// serve file as is
	return _createFileResponse(requestPath);
	
}

Response ResponseHandler::handleGETDirectory( Request req, std::pair<ServerConfig *, Location *> config, const std::string& requestPath ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;

	// check redir
	if (requestPath.back() != '/') {
		return _createRedirectionResponse(MovedPermanently, requestPath + "/");
	}

	// check for index files
	std::string _indexPath = FileHandler::searchIndexes(requestPath, _loc->_index_file);
	if (_indexPath.empty() && !_loc->_autoindex) {
		return _createErrorResponse(Forbidden, config);
	}

	if (_indexPath.empty() && _loc->_autoindex) {
		std::string _uri = _conf->getServerIp() + ":" + toString<int>(_conf->getPort());
		return _createDirListingResponse(_uri, _loc->_root, requestPath);
	}

	// if index readable
	if (FileHandler::isPathReadable(_indexPath)) {
		return _createErrorResponse(Forbidden, config);
	}

	// if index not empty
	if (FileHandler::requiresCGI(_indexPath)) {
		return _createFileCGIResponse(req, _conf, _loc, _indexPath);
	}

	// serve file as is
	return _createFileResponse(_indexPath);
}

std::pair<bool, Response> ResponseHandler::handleRequestErrors( Request req, std::pair<ServerConfig *, Location *> config ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;

	// If Request is not valid
	if (false /* TODO: check if req.statusCode is not 0 */) {
		Response r = _createErrorResponse(BadRequest, config); // 400
		return std::make_pair(true, r);
	}

	// If Transfer-Encoding is something other than chunked
	if (!req.getHeader(H_TRANSFER_ENCODING).empty() && req.getHeader(H_TRANSFER_ENCODING) != "chunked") {
		Response r = _createErrorResponse(NotImplemented, config); // 501
		return std::make_pair(true, r);
	}

	// If both Transfer-Encoding & Content-Length not present
	if (req.getHeader(H_TRANSFER_ENCODING).empty() && req.getHeader(H_CONTENT_LENGTH).empty()) {
		Response r = _createErrorResponse(BadRequest, config); // 400
		return std::make_pair(true, r);
	}

	// If request PATH too long
	if (req.getPath().length() > 2048) {
		Response r = _createErrorResponse(URITooLong, config); // 414
		return std::make_pair(true, r);
	}

	// if bodySize > clientmaxbuffersize
	// TODO: fix and uncomment
	// if (_conf != NULL && _conf->getClientBufferSize() > 0
	// 	/*&& req.bodySize() > _conf->getClientBufferSize()*/) {
	// 	Response r = _createErrorResponse(PayloadTooLarge, config); // 413
	// 	return std::make_pair(true, r);
	// }

	// If no location matches request
	if (_loc == NULL) {
		Response r = _createErrorResponse(NotFound, config); // 404
		return std::make_pair(true, r);
	}

	// If location has redirection !Not an error but :)
	if (!_loc->_redirection_path.empty()) {
		// check if redir path start with /, else add it
		if (_loc->_redirection_path.front() != '/')
			_loc->_redirection_path.insert(_loc->_redirection_path.begin(), '/');
		Response r = _createRedirectionResponse(MovedPermanently, _loc->_redirection_path);
		return std::make_pair(true, r);
	}

	// Check if method is allowed
	if (!_loc->_allow_methods.empty() && !isMethodAllowed(_loc->_allow_methods, req.getMethod())) {
		Response r = _createErrorResponse(MethodNotAllowed, config); // 405
		return std::make_pair(true, r);
	}

	// Check if method (DELETE) is explicitely allowed
	if (_loc->_allow_methods.empty() && toUpperCase(trim(req.getMethod())) == DELETE) {
		Response r = _createErrorResponse(MethodNotAllowed, config); // TODO: verify if 405 is right or Forbidden
		return std::make_pair(true, r);
	}

	// Check if method in GET, POST, DELETE
	if (!isMethodImplemented(req.getMethod())) {
		Response r = _createErrorResponse(NotImplemented, config); // 501
		return std::make_pair(true, r);
	}

	// all good, return false to continue handling request
	return std::make_pair(false, Response());
}

std::pair<ServerConfig *, Location *> ResponseHandler::getMatchingConfig( Request req, std::vector<ServerConfig *> servers ) {
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

	// no match has been found :(
	return std::make_pair((ServerConfig *)NULL, (Location *)NULL);
}

Response ResponseHandler::handleRequests( Request req, std::pair<ServerConfig *, Location *> config ) {
	// check for request errors
	std::pair<bool, Response> _res = handleRequestErrors(req, config);
	if (_res.first) return _res.second;

	// no errors / redirections
	std::string _m = toUpperCase(trim(req.getMethod()));

	if (_m == GET) {
		return handleGETRequest(req, config);
	} else if (_m == POST) {
		// TODO: handle POST
	} else if (_m == DELETE) {
		// TODO: handle DELETE
	} else {
		// normally it shouldn't get here but if so:
		return _createErrorResponse(MethodNotAllowed, config); 
	}
}

Response ResponseHandler::handleGETRequest( Request req, std::pair<ServerConfig *, Location *> config ) {
	// check for request errors
	std::pair<bool, Response> _res = handleRequestErrors(req, config);
	if (_res.first) return _res.second;

	ServerConfig* _conf = config.first;
	Location* _loc = config.second;

	std::string _requestPath = FileHandler::getFullPath(_loc->_root, req.getPath());

	// check if path doesn't exist
	if (!FileHandler::pathExists(_requestPath)) {
		return _createErrorResponse(NotFound, config);
	}

	// check if path not readable
	if (FileHandler::isPathReadable(_requestPath)) {
		return _createErrorResponse(Forbidden, config);
	}

	// check if path is neither dir nor file
	FileType _type = FileHandler::getType(_requestPath);
	if (_type != T_DIR && _type != T_FILE) {
		return _createErrorResponse(InternalServerError, config);
	}

	// check if path is file or dir
	if (_type == T_DIR) {
		return handleGETDirectory(req, config, _requestPath);
	}

	if (_type == T_FILE) {
		return handleGETFile(req, config, _requestPath);
	}

	// this shouldn't happen
	return _createErrorResponse(InternalServerError, config);
}






