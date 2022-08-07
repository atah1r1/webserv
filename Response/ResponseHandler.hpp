/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 14:35:35 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/07 23:11:16 by atahiri          ###   ########.fr       */
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
#include "../CGI/CGI.hpp"

class ResponseHandler {
private:
	static std::string _getDefaultErrorBody( int statusCode, std::pair<ServerConfig *, Location *> config );
	static std::string _getStandardErrorBody( int statusCode );
	static std::string _getDirListingBody( const std::string& uri, const std::string& root, const std::string& dirPath );
	static Response _createErrorResponse(int statusCode, std::pair<ServerConfig *, Location *> config, const std::string& temp );
	static Response _createBodylessErrorResponse( int statusCode, std::pair<ServerConfig *, Location *> config, const std::string& temp );
	static Response _createDirListingResponse( const std::string& uri, const std::string& root, const std::string& dirPath );
	static Response _createRedirectionResponse( int statusCode, const std::string& dirPath );
	static Response _createFileResponse( const std::string& filePath, std::pair<ServerConfig *, Location *> config );
	static Response _createFileCGIResponse( Request req, ServerConfig *conf, Location * loc, const std::string& filePath );
	static Response _handleGETFile( Request req, std::pair<ServerConfig *, Location *> config, const std::string& requestPath  );
	static Response _handleGETDirectory( Request req, std::pair<ServerConfig *, Location *> config, const std::string& requestPath );
	static std::pair<bool, Response> _handleRequestErrors( Request req, std::pair<ServerConfig *, Location *> config) ;
	static std::pair<ServerConfig *, Location *> _getMatchingConfig( Request req, ServerConfig serverConfig );
public:

	static Response handleRequests( Request req, ServerConfig servers );
	static Response handleGETRequest( Request req, std::pair<ServerConfig *, Location *> config );
	static Response handleDELETERequest( Request req, std::pair<ServerConfig *, Location *> config );
	static Response handlePOSTRequest( Request req, std::pair<ServerConfig *, Location *> config );
};

#endif // __RESPONSE_HANDLER_HPP__
