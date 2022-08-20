/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 22:24:39 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/20 18:58:06 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHandler.hpp"

/*
** BODY GENERATORS
*/
std::string ResponseHandler::_getDefaultErrorBody( int statusCode, const std::pair<ServerConfig *, Location *>& config ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;

	if (_conf == NULL || _loc == NULL) {
		return _getStandardErrorBody(statusCode);
	}

	std::string _root = !_loc->_root.empty() ? _loc->_root : _conf->getRoot();

	std::string _errorPage = _conf->getErrorPage(statusCode);

	if (!_errorPage.empty()) {
		std::string _pagePath = FileHandler::getFullPath(_root, _errorPage);
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

std::string ResponseHandler::_getDirListingBody( const std::string& root, const std::string& dirPath ) {
	std::stringstream ss;

	std::string _dirPath = FileHandler::disconnectPath(root, dirPath);
	errno = 0;
	std::map<std::string, std::string> _paths = FileHandler::exploreDir(root, dirPath);
	if (errno != 0 && _paths.empty())
		return "";
	ss << "<html>\n";
	ss << "<head><title>" << "Directory listing for " << _dirPath << "</title></head>\n";
	ss << "<body bgcolor=\"white\">\n";
	ss << "<h1>" << "Directory listing for " << _dirPath << "</h1>\n";
	ss << "<hr>\n<ul>\n";

	std::map<std::string, std::string>::iterator it = _paths.begin();
	while (it != _paths.end()) {
		ss << "<li><a href=\"" << it->second << "\">" << it->first << "</a></li>\n";
		++it;
	}
	ss << "</ul>\n<hr>\n";
	ss << "</body>\n";
	ss << "</html>\n";

	return ss.str();
}

/*
** RESPONSE GENERATORS
*/
Response ResponseHandler::_createErrorResponse( const Request& req, int statusCode, const std::pair<ServerConfig *, Location *>& config, const std::string& temp ) {
	Response r;

	std::string body = _getDefaultErrorBody(statusCode, config) + temp;

	r.setVersion(HTTP_VERSION);
	r.setStatusCode(statusCode);
	r.setStatus(getReason(statusCode));
	r.addHeader(H_SERVER, SERVER_VERSION);
	r.addHeader(H_DATE, getCurrentDate());
	r.addHeader(H_CONTENT_TYPE, "text/html");
	r.addHeader(H_CONTENT_LENGTH, toString<size_t>(body.length()));
	std::string _conn = req.getHeader(H_CONNECTION);
	r.addHeader(H_CONNECTION, !_conn.empty() ? _conn : "keep-alive");
	r.setBody(body);

	return r;
}

Response ResponseHandler::_createBodylessErrorResponse( const Request& req, int statusCode, const std::pair<ServerConfig *, Location *>& config, const std::string& temp ) {
	Response r;

	(void)config;
	(void)temp;

	r.setVersion(HTTP_VERSION);
	r.setStatusCode(statusCode);
	r.setStatus(getReason(statusCode));
	r.addHeader(H_SERVER, SERVER_VERSION);
	r.addHeader(H_DATE, getCurrentDate());
	std::string _conn = req.getHeader(H_CONNECTION);
	r.addHeader(H_CONNECTION, !_conn.empty() ? _conn : "keep-alive");

	return r;
}

Response ResponseHandler::_createDirListingResponse( const Request& req, const std::pair<ServerConfig *, Location *>& config, const std::string& dirPath ) {
	std::string root = config.second->_root;
	Response r;
	std::string body = _getDirListingBody(root, dirPath);

	if (body.empty()) {
		r.clearAll();
		return _createErrorResponse(req, 500, config, "EXPORE DIR FAILED!");
	}

	r.setVersion(HTTP_VERSION);
	r.setStatusCode(OK);
	r.setStatus(getReason(OK));
	r.addHeader(H_SERVER, SERVER_VERSION);
	r.addHeader(H_DATE, getCurrentDate());
	r.addHeader(H_CONTENT_TYPE, "text/html");
	r.addHeader(H_CONTENT_LENGTH, toString<size_t>(body.length()));
	std::string _conn = req.getHeader(H_CONNECTION);
	r.addHeader(H_CONNECTION, !_conn.empty() ? _conn : "keep-alive");

	r.setBody(body);

	return r;
}

Response ResponseHandler::_createRedirectionResponse( const Request& req, int statusCode, const std::pair<ServerConfig *, Location *>& config, const std::string& dirPath ) {
	Response r;
	std::string body = _getDefaultErrorBody(statusCode, config);

	r.setVersion(HTTP_VERSION);
	r.setStatusCode(statusCode);
	r.setStatus(getReason(statusCode));

	r.addHeader("Server", SERVER_VERSION);
	r.addHeader(H_DATE, getCurrentDate());
	r.addHeader(H_LOCATION,  dirPath);
	r.addHeader(H_CONTENT_TYPE, "text/html");
	r.addHeader(H_CONTENT_LENGTH, toString<size_t>(body.length()));
	std::string _conn = req.getHeader(H_CONNECTION);
	r.addHeader(H_CONNECTION, !_conn.empty() ? _conn : "keep-alive");

	r.setBody(body);

	return r;
}

Response ResponseHandler::_createFileResponse( const Request& req, const std::string& filePath, const std::pair<ServerConfig *, Location *>& config ) {
	Response r;

	// setup file for reading
	r.setFilePath(filePath);
	if (!r.setupFile())
		return _createErrorResponse(req, InternalServerError, config, std::string("COULDN'T NOT OPEN FILE<br/>" + filePath));
	r.setBuffered(true);
	r.setVersion(HTTP_VERSION);
	r.setStatusCode(OK);
	r.setStatus(getReason(OK));
	r.addHeader(H_SERVER, SERVER_VERSION);
	r.addHeader(H_DATE, getCurrentDate());
	std::string _mime = getMimeType(FileHandler::getFileExtension(filePath));
	if (!_mime.empty())
		r.addHeader(H_CONTENT_TYPE, _mime);
	r.addHeader(H_CONTENT_LENGTH, toString<size_t>(FileHandler::getFileSize(filePath)));
	std::string _conn = req.getHeader(H_CONNECTION);
	r.addHeader(H_CONNECTION, !_conn.empty() ? _conn : "keep-alive");

	return r;
}

Response ResponseHandler::_createFileCGIResponse( const Request& req, ServerConfig *conf, Location * loc, const std::string& filePath ) {
	std::cerr << "HANDING CGI..." << std::endl;
	char buff[1024] = {0};
	std::string _root = !loc->_root.empty() ? loc->_root : conf->getRoot();

	std::vector<const char *> v;
	v.push_back(strdup((std::string("REQUEST_METHOD=") + req.getMethod()).c_str()));
	v.push_back(strdup((std::string("PATH=") + (getenv("PATH") ? getenv("PATH") : "")).c_str()));
	v.push_back(strdup("AUTH_TYPE=null"));
	v.push_back(strdup((std::string("TERM=") + (getenv("TERM") ? getenv("TERM") : "")).c_str()));
	v.push_back(strdup((std::string("HOME=") + (getenv("HOME") ? getenv("HOME") : "")).c_str()));
	gethostname(buff, 1023);
	v.push_back(strdup((std::string("HOSTNAME=") + buff).c_str()));
	getlogin_r(buff, 1023);
	v.push_back(strdup((std::string("USER=") + buff).c_str()));
	v.push_back(strdup((std::string("CONTENT_LENGTH=") + req.getHeader(H_CONTENT_LENGTH)).c_str()));
	v.push_back(strdup((std::string("CONTENT_TYPE=") + req.getHeader(H_CONTENT_TYPE)).c_str()));
	v.push_back(strdup(std::string("GATEWAY_INTERFACE=CGI/1.1").c_str()));
	v.push_back(strdup((std::string("PATH_INFO=") + req.getPath()).c_str()));
	v.push_back(strdup((std::string("PATH_TRANSLATED=") + filePath).c_str()));
	v.push_back(strdup((std::string("REMOTE_ADDR=") + conf->getServerIp()).c_str()));
	v.push_back(strdup((std::string("REMOTE_HOST=") + req.getHeader("host").substr(0, req.getHeader("host").find_first_of(':'))).c_str()));
	v.push_back(strdup((std::string("SERVER_NAME=") + conf->getServerNames().front()).c_str()));
	v.push_back(strdup((std::string("SERVER_PORT=") + toString<int>(conf->getPort())).c_str()));
	v.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
	v.push_back(strdup("SERVER_SOFTWARE=" SERVER_VERSION));
	v.push_back(strdup((std::string("QUERY_STRING=") + req.getQueries()).c_str()));
	v.push_back(strdup("REDIRECT_STATUS="));
	v.push_back(NULL);

	// get cgi path.
	std::string _cgiPath = getCGIPath( loc->_cgis, FileHandler::getFileExtension(filePath));

	// execute cgi on filePath and get filePath of response.
	std::string _filePath = CGI::execute(_cgiPath, filePath, req.getBodyFileName(), const_cast<char *const*>(v.data()));

	// if no file was returned, return error. 500
	if (_filePath.empty())
		return _createErrorResponse(req, InternalServerError, std::pair<ServerConfig *, Location *>(conf, loc), "CGI ERROR<br/>");

	// parse the response file and return the response.
	std::pair<bool, Response> res = Response::parseFrom(_filePath);

	// if parsing failed return error. 500
	if (!res.first) {
		res.second.clearAll();
		return _createErrorResponse(req, InternalServerError, std::pair<ServerConfig *, Location *>(conf, loc), "COULDN'T NOT OPEN FILE");
	}

	// add headers to response.
	res.second.addHeader(H_SERVER, SERVER_VERSION);
	std::string _conn = req.getHeader(H_CONNECTION);
	res.second.addHeader(H_CONNECTION, !_conn.empty() ? _conn : "keep-alive");

	// return response.
	return res.second;
}

/*
** CONFIG/ERRORS HANDLERS
*/
std::pair<ServerConfig *, Location *> ResponseHandler::_getMatchingConfig( const Request& req, std::vector<ServerConfig>& serverConfigs ) {

	// match Server & Location with same HOST & PATH
	for(std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it) {
		if (isSameHost(it->getServerIp(), req.getHost()) && it->getPort() == req.getPort()) {
			std::vector<Location *> _locations = it->getLocations();
			Location* _l = matchLocation(_locations, req.getPath());
			if (_l != NULL)
				return std::make_pair(&(*it), _l);
		}
	}

	// match Server & Location with same SERVERNAME & PATH
	for(std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it) {
		if ( matchServerName(it->getServerNames(), req.getHost()) && it->getPort() == req.getPort()) {
			std::vector<Location *> _locations = it->getLocations();
			Location* _l = matchLocation(_locations, req.getPath());
			if (_l != NULL)
				return std::make_pair(&(*it), _l);
		}
	}

	// no match has been found :(
	return std::make_pair(&serverConfigs.front(), (Location *)NULL);
}

std::pair<bool, Response> ResponseHandler::_handleRequestErrors( const Request& req, const std::pair<ServerConfig *, Location *>& config ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;

	// If Request is not valid
	if (req.getStatusCode() != 0) {
		Response r = _createErrorResponse(req, req.getStatusCode(), config, "REQ NOT VALID\n"); // 400
		return std::make_pair(true, r);
	}

	// If Transfer-Encoding is something other than chunked
	if (!req.getHeader(H_TRANSFER_ENCODING).empty() && req.getHeader(H_TRANSFER_ENCODING) != "chunked") {
		Response r = _createErrorResponse(req, NotImplemented, config, "TRANS-ENC != chunked\n"); // 501
		return std::make_pair(true, r);
	}

	// If both Transfer-Encoding & Content-Length not present
	if (trim(toUpperCase(req.getMethod())) == POST 
		&& req.getHeader(H_TRANSFER_ENCODING).empty() && req.getHeader(H_CONTENT_LENGTH).empty()) {
		Response r = _createErrorResponse(req, BadRequest, config, "TRANS-ENC = \"\" & CONT-LEN = \"\"\n"); // 400
		return std::make_pair(true, r);
	}

	// If request PATH too long
	if (req.getPath().length() > 2048) {
		Response r = _createErrorResponse(req, URITooLong, config, "URL TOO LONG\n"); // 414
		return std::make_pair(true, r);
	}

	// if bodySize > clientmaxbuffersize
	size_t _bodySize = !req.getBodyFileName().empty() ? FileHandler::getFileSize(req.getBodyFileName()) : 0;
	if (_conf != NULL && _conf->getClientBufferSize() > 0 && _bodySize > (size_t)_conf->getClientBufferSize()) {
		Response r = _createErrorResponse(req, PayloadTooLarge, config, "MAXBODY SIZE"); // 413
		return std::make_pair(true, r);
	}

	// If no location matches request
	if (_conf == NULL || _loc == NULL) {
		Response r = _createErrorResponse(req, NotFound, config, "LOC NO MATCH\n"); // 404
		return std::make_pair(true, r);
	}

	// Check if method is allowed
	std::vector<std::string> _allowedMethods = !_loc->_allow_methods.empty() ? _loc->_allow_methods : _conf->getAllowMethods();
	if (!_allowedMethods.empty() && !isMethodAllowed(_allowedMethods, req.getMethod())) {
		Response r = _createErrorResponse(req, MethodNotAllowed, config, "METH NOT ALLOWED\n"); // 405
		return std::make_pair(true, r);
	}

	// Check if method (DELETE) is explicitely allowed
	if (toUpperCase(trim(req.getMethod())) == DELETE && !isMethodAllowed(_allowedMethods, req.getMethod())) {
		Response r = _createErrorResponse(req, MethodNotAllowed, config, "DELETE NOT EXP ALLOWED\n");
		return std::make_pair(true, r);
	}

	// Check if method in GET, POST, DELETE
	if (!isMethodImplemented(req.getMethod())) {
		Response r = _createErrorResponse(req, NotImplemented, config, "METH NOT IMPL.\n"); // 501
		return std::make_pair(true, r);
	}

	// all good, return false to continue handling request
	return std::make_pair(false, Response());
}

/*
** REQUEST HANDLERS
*/
Response ResponseHandler::_handleGETFile( const Request& req, const std::pair<ServerConfig *, Location *>& config, const std::string& requestPath  ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;

	// cgi
	if (FileHandler::requiresCGI(_loc->_cgis, requestPath)) {
		return _createFileCGIResponse(req, _conf, _loc, requestPath);
	}

	// serve file as is
	return _createFileResponse(req, requestPath, config);
	
}

Response ResponseHandler::_handleGETDirectory( const Request& req, const std::pair<ServerConfig *, Location *>& config, const std::string& requestPath ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;

	// Select default configs
	std::string _root = !_loc->_root.empty() ? _loc->_root : _conf->getRoot();
	bool _autoIndexing = _loc->_autoindex;
	std::vector<std::string> _indexFiles = !_loc->_index_file.empty() ? _loc->_index_file : _conf->getIndexFiles();

	// check redir
	if (requestPath.back() != '/') {
		return _createRedirectionResponse(req, MovedPermanently, config, FileHandler::disconnectPath(_root, requestPath) + "/");
	}

	// check for index files
	std::string _indexPath = FileHandler::searchIndexes(requestPath, _indexFiles);
	if (_indexPath.empty() && !_autoIndexing) {
		return _createErrorResponse(req, Forbidden, config, "INDEXS EMPTY & AUTOINDEX OFF\n");
	}

	if (_indexPath.empty() && _autoIndexing) {
		return _createDirListingResponse(req, config, requestPath);
	}

	// if index readable
	if (!FileHandler::isPathReadable(_indexPath)) {
		return _createErrorResponse(req, Forbidden, config, "DIR: PATH NOT READABLE\n");
	}

	// if index not empty
	if (FileHandler::requiresCGI(_loc->_cgis, _indexPath)) {
		return _createFileCGIResponse(req, _conf, _loc, _indexPath);
	}

	// serve file as is
	return _createFileResponse(req, _indexPath, config);
}

Response ResponseHandler::handleRequests( const Request& req, std::vector<ServerConfig>& serverConfigs ) {
	std::pair<ServerConfig *, Location *> config = _getMatchingConfig(req, serverConfigs);

	// check for request errors
	std::pair<bool, Response> _res = _handleRequestErrors(req, config);
	if (_res.first) return _res.second;

	ServerConfig* _conf = config.first;
	Location* _loc = config.second;
	std::string _root = !_loc->_root.empty() ? _loc->_root : _conf->getRoot();

	// loc has redirection
	if (!_loc->_redirection_path.empty()) {
		std::string _redir = trim(_loc->_redirection_path);

		if (_redir.front() == '/') {
			_redir = std::string("http://") + req.getHost() + ":" + toString(req.getPort()) + _redir;
		}
		return _createRedirectionResponse(req, MovedPermanently, config, _redir);
	}

	// no errors / redirections
	std::string _m = toUpperCase(trim(req.getMethod()));

	if (_m == GET) {
		return handleGETRequest(req, config);
	} else if (_m == POST) {
		return handlePOSTRequest(req, config);
	} else if (_m == DELETE) {
		return handleDELETERequest(req, config);
	}
	// normally it shouldn't get here but if so:
	return _createErrorResponse(req, MethodNotAllowed, config, "IT SHOULD'T GET HERE\n"); 
}

Response ResponseHandler::handleGETRequest( const Request& req, const std::pair<ServerConfig *, Location *>& config ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;
	std::string _root = !_loc->_root.empty() ? _loc->_root : _conf->getRoot();

	std::string _requestPath = FileHandler::getFullPath(_root, req.getPath());
	_requestPath = decodeUrl(_requestPath);

	// check if path doesn't exist
	if (!FileHandler::pathExists(_requestPath)) {
		return _createErrorResponse(req, NotFound, config, std::string("PATH NOT EXIST\n") + _requestPath);
	}

	// check if path not readable
	if (!FileHandler::isPathReadable(_requestPath)) {
		return _createErrorResponse(req, Forbidden, config, "PATH NOT READABLE\n");
	}

	// check if path is neither dir nor file
	FileType _type = FileHandler::getType(_requestPath);
	if (_type != T_DIR && _type != T_FILE) {
		return _createErrorResponse(req, InternalServerError, config, "PATH == T_OTHER | T_ERROR\n");
	}

	// check if path is file or dir
	if (_type == T_DIR) {
		return _handleGETDirectory(req, config, _requestPath);
	}

	if (_type == T_FILE) {
		return _handleGETFile(req, config, _requestPath);
	}

	// this shouldn't happen
	return _createErrorResponse(req, InternalServerError, config, "SHOUDN'T HAPPEN TOO\n");
}

Response ResponseHandler::handleDELETERequest( const Request& req, const std::pair<ServerConfig *, Location *>& config ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;
	std::string _root = !_loc->_root.empty() ? _loc->_root : _conf->getRoot();

	std::string _requestPath = FileHandler::getFullPath(_root, req.getPath());
	_requestPath = decodeUrl(_requestPath);

	// incase of file with '/' at the end
	std::string _temp = _requestPath.back() == '/' ? _requestPath.substr(0, _requestPath.size() - 1) : _requestPath;

	// check if path 
	FileType _type = FileHandler::getTypeS(_temp);

	if (_type == T_ERROR && errno == ENOENT) {
		return _createErrorResponse(req, NotFound, config, std::string("PATH NOT EXIST\n") + _requestPath);
	}

	if ((_type == T_DIR && _requestPath.back() != '/') ||
		(_type == T_FILE && _requestPath.back() == '/')) {
		return _createErrorResponse(req, Conflict, config, "(FILE + /) or (DIR - /)\n");
	}

	// check if path is file or dir
	if (_type == T_FILE && FileHandler::requiresCGI(_loc->_cgis, _requestPath)) {
		return _createFileCGIResponse(req, _conf, _loc, _requestPath);
	}

	bool _removed = FileHandler::removeAll(_requestPath);

	if (_removed)
		return _createBodylessErrorResponse(req, NoContent, config, "GOOD");

	return _createErrorResponse(req, InternalServerError, config, strerror(errno));
}

Response ResponseHandler::handlePOSTRequest( const Request& req, const std::pair<ServerConfig *, Location *>& config ) {
	ServerConfig* _conf = config.first;
	Location* _loc = config.second;
	std::string _root = !_loc->_root.empty() ? _loc->_root : _conf->getRoot();

	std::string _requestPath = FileHandler::getFullPath(_root, req.getPath());
	_requestPath = decodeUrl(_requestPath);

	if (!_loc->_upload_store.empty()) {

		std::string _storePath = FileHandler::getFullPath(_root, _loc->_upload_store);

		if (!FileHandler::pathExists(_storePath) || FileHandler::getType(_storePath) != T_DIR) {
			return _createErrorResponse(req, InternalServerError, config, "STORE PATH = FILE OR DON'T EXIST\n");
		}

		std::string _ext = getExtension(req.getHeader(H_CONTENT_TYPE));
		std::string _fileName = FileHandler::getFullPath(_storePath, "file_" + randomFileName(false) + (_ext.empty() ? "" : "." + _ext));

		std::cerr << "FILE NAME: " << _fileName << std::endl;

		std::ifstream  src(req.getBodyFileName(), std::ifstream::binary | std::ifstream::in );
    	std::ofstream  dst(_fileName, std::ios::binary | std::ofstream::out | std::ios::trunc );
		if (!src.is_open() || !dst.is_open()) {
			return _createErrorResponse(req, InternalServerError, config, "SRC || DST FILE NOT OPEN\n");
		}
		dst << src.rdbuf();
		src.close();
		dst.close();

		remove(req.getBodyFileName().c_str());

		return _createBodylessErrorResponse(req, Created, config, "CREATED! SUCCESS\n");
	}

	if (!FileHandler::pathExists(_requestPath)) {
		return _createErrorResponse(req, NotFound, config, "PATH NOT FOUND");
	}

	std::string _temp = _requestPath.back() == '/' ? _requestPath.substr(0, _requestPath.size() - 1) : _requestPath;
	FileType _type = FileHandler::getType(_temp);

	if (_type == T_DIR) {

		if (_requestPath.back() != '/') {
			return _createRedirectionResponse(req, MovedPermanently, config, _requestPath + "/");
		}

		std::string _indexPath = FileHandler::searchIndexes(_requestPath, _loc->_index_file);
		if (_indexPath.empty()) {
			return _createErrorResponse(req, Forbidden, config, "INDEX EMPTY\n");
		}

		if (!FileHandler::requiresCGI(_loc->_cgis, _indexPath)) {
			return _createErrorResponse(req, Forbidden, config, "INDEX DON'T REQUIRE CGI\n");
		}

		return _createFileCGIResponse(req, _conf, _loc, _indexPath);
	}

	if (_type == T_FILE) {

		if (!FileHandler::requiresCGI(_loc->_cgis, _requestPath)) {
			return _createErrorResponse(req, Forbidden, config, "FILE DON'T REQUIRE CGI\n");
		}

		return _createFileCGIResponse(req, _conf, _loc, _requestPath);
	}

	return _createErrorResponse(req, InternalServerError, config, "PATH NOT FILE NOR DIR\n");
}
