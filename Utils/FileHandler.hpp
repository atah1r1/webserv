/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 00:53:03 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/18 01:30:31 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILE_HANDLER_HPP__
#define __FILE_HANDLER_HPP__

#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <map>
#include "Debug.hpp"
#include "Utils.hpp"
#include "../Config/ServerConfig.hpp"
#include "../Includes/Webserv.hpp"

enum FileType {
	T_FILE = 0,
	T_DIR = 1,
	T_OTHER = 2,
	T_LINK = 3,
	T_ERROR = 4
};

class FileHandler {
public:
	static std::string getFullPath( const std::string& root, const std::string& path );
	static std::string disconnectPath( const std::string& root, const std::string& path );
	static std::string getFileExtension( const std::string& fileName );
	static std::string getLastModificationDate( const std::string& path );
	static std::string searchIndexes( const std::string& dirPath, std::vector<std::string> indexFiles );
	static std::string readFile( const std::string& filePath );

	static std::map<std::string, std::string> exploreDir( const std::string& root, const std::string& dirPath );
	static FileType getType( const std::string& path );
	static FileType getTypeS( const std::string& path );
	static size_t getFileSize(const std::string& path );
	static bool pathExists( const std::string& path );
	static bool isPathReadable( const std::string& path );
	static bool isPathWritable( const std::string& path );
	static bool requiresCGI( const std::map<std::string, std::string>& cgiPaths, const std::string& path );
	static bool removeAll( const std::string& path );
};

#endif
