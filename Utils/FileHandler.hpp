/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 00:53:03 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/29 03:25:41 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILE_HANDLER_HPP__
#define __FILE_HANDLER_HPP__

#include <iostream>
#include <sstream>
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
	T_ERROR = 3
};

class FileHandler {
public:
	static std::string getFullPath( const std::string& root, const std::string& path );
	static std::string disconnectPath( const std::string& root, const std::string& path );
	static std::string getFileExtension( const std::string& fileName );
	static std::string getLastModificationDate( const std::string& path );
	static std::string searchIndexes( const std::string& dirPath, std::vector<std::string> indexFiles );
	static std::map<std::string, std::string> exploreDir( const std::string& root, const std::string& dirPath );
	static FileType getType( const std::string& path );
	static inline bool pathExists( const std::string& path );
	static inline bool isPathReadable( const std::string& path );
	static inline bool isPathWritable( const std::string& path );
	static inline bool requiresCGI( const std::string& path );
};

#endif