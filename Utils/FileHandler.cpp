/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 00:57:02 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/29 03:27:09 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileHandler.hpp"

// TODO: after return if vector is empty, check for ERRNO errors.
std::map<std::string, std::string> FileHandler::exploreDir( const std::string& root, const std::string& dirPath ) {
	DIR *dir;
	struct dirent *entry;
	struct stat info;
	std::map<std::string, std::string> _paths;

	dir = opendir(dirPath.c_str());

    if (!dir) return _paths;

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] != '.') {
			std::string _pathName = entry->d_name;
			if (getType(getFullPath(dirPath, entry->d_name)) == T_DIR) {
				_pathName.push_back('/');
			}
			std::string _path = disconnectPath(root, dirPath);
			_paths.insert(_pathName, getFullPath(_path, _pathName));
		}
	}
	closedir(dir);
	return _paths;
}

std::string FileHandler::getFullPath( const std::string& root, const std::string& path ) {
	std::string _root = trim(root);
	std::string _path = trim(path);

	if (_root.back() == '/' && _path.front() == '/') {
		return _root.substr(0, _root.length() - 1).append(_path);
	} else if (_root.back() != '/' && _path.front() != '/') {
		return _root.append("/").append(_path);
	}
	return _root.append(_path);
}

std::string FileHandler::disconnectPath( const std::string& root, const std::string& path ) {
	std::string _path;
	if (beginsWith(path, root)) {
		_path = path.substr(root.length());
	}
	if (_path.empty() || _path.front() != '/') {
		_path.insert(_path.begin(), '/');
	}
	return (_path);
}

std::string FileHandler::getFileExtension( const std::string& fileName ) {
	int index = fileName.find_last_of(".");
	if (index != -1) {
		std::string ext = fileName.substr(index + 1);
		return ext;
	}
	return "";
}

std::string	FileHandler::getLastModificationDate( const std::string& path ) {
	struct stat st;
	std::stringstream date;

	if( stat(path.c_str(), &st) != 0 ) return "";
	tm *ltm = gmtime(&st.st_mtime);

	date << day_names[ltm->tm_wday] << ", " << ltm->tm_mday << " "
		<< month_names[ltm->tm_mon] << " " << (ltm->tm_year + 1900) << " " 
		<< (ltm->tm_hour) % 24 << ":" << ltm->tm_min << ":" << ltm->tm_sec << " GMT";
	return date.str();
}

std::string FileHandler::searchIndexes( const std::string& dirPath, std::vector<std::string> indexFiles ) {
	std::vector<std::string>::iterator it = indexFiles.begin();
	while (it != indexFiles.end())
	{
		std::string _fullPath = getFullPath(dirPath, *it);
		if (pathExists(_fullPath)) return _fullPath;
	}
	return "";
}

FileType FileHandler::getType( const std::string& path ) {
	struct stat s;

	if( stat(path.c_str(), &s) == 0 )
	{
		if( s.st_mode & S_IFDIR )
			return T_DIR;
		else if( s.st_mode & S_IFREG )
			return T_FILE;
		else
			return T_OTHER;
	}
	return T_ERROR;
}

inline bool FileHandler::requiresCGI( const std::string& path ) {
	return !getCGIPath(getFileExtension(path)).empty();
}

inline bool FileHandler::pathExists( const std::string& path ) {
	return ( access( path.c_str(), F_OK ) != -1 );
}

inline bool FileHandler::isPathReadable( const std::string& path ) {
	return ( access( path.c_str(), R_OK ) != -1 );
}

inline bool FileHandler::isPathWritable( const std::string& path ) {
	return ( access( path.c_str(), W_OK ) != -1 );
}
