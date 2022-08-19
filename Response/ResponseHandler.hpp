/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 14:35:35 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/19 18:03:28 by ehakam           ###   ########.fr       */
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
	static std::string _getDefaultErrorBody( int statusCode, const std::pair<ServerConfig *, Location *>& config );
	static std::string _getStandardErrorBody( int statusCode );
	static std::string _getDirListingBody( const std::string& root, const std::string& dirPath );
	static Response _createErrorResponse( const Request& req, int statusCode, const std::pair<ServerConfig *, Location *>& config, const std::string& temp );
	static Response _createBodylessErrorResponse( const Request& req, int statusCode, const std::pair<ServerConfig *, Location *>& config, const std::string& temp );
	static Response _createDirListingResponse( const Request& req, const std::string& root, const std::string& dirPath );
	static Response _createRedirectionResponse( const Request& req, int statusCode, const std::pair<ServerConfig *, Location *>& config, const std::string& dirPath );
	static Response _createFileResponse( const Request& req, const std::string& filePath, const std::pair<ServerConfig *, Location *>& config );
	static Response _createFileCGIResponse( const Request& req, ServerConfig *conf, Location * loc, const std::string& filePath );
	static Response _handleGETFile( const Request& req, const std::pair<ServerConfig *, Location *>& config, const std::string& requestPath  );
	static Response _handleGETDirectory( const Request& req, const std::pair<ServerConfig *, Location *>& config, const std::string& requestPath );
	static std::pair<bool, Response> _handleRequestErrors( const Request& req, const std::pair<ServerConfig *, Location *>& config) ;
	static std::pair<ServerConfig *, Location *> _getMatchingConfig( const Request& req, std::vector<ServerConfig>& serverConfigs );
public:

	static Response handleRequests( const Request& req, std::vector<ServerConfig>& serverConfigs );
	static Response handleGETRequest( const Request& req, const std::pair<ServerConfig *, Location *>& config );
	static Response handleDELETERequest( const Request& req, const std::pair<ServerConfig *, Location *>& config );
	static Response handlePOSTRequest( const Request& req, const std::pair<ServerConfig *, Location *>& config );
};

#endif // __RESPONSE_HANDLER_HPP__
