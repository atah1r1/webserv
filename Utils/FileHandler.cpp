/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 00:57:02 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/20 19:01:58 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileHandler.hpp"

std::map<std::string, std::string> FileHandler::exploreDir( const std::string& root, const std::string& dirPath ) {
	DIR *dir;
	struct dirent *entry;
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
			_paths[_pathName] = getFullPath(_path, _pathName);
			//_paths.insert( std::make_pair<std::string, std::string>(_pathName, getFullPath(_path, _pathName)));
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

	date << day_names[ltm->tm_wday] << ", "
		<< std::setfill('0') << std::setw(2) << ltm->tm_mday << " "
		<< month_names[ltm->tm_mon] << " "
		<< std::setfill('0') << std::setw(4) << (ltm->tm_year + 1900) << " " 
		<< std::setfill('0') << std::setw(2) << (ltm->tm_hour % 24) << ":" 
		<< std::setfill('0') << std::setw(2) << ltm->tm_min << ":"
		<< std::setfill('0') << std::setw(2) << ltm->tm_sec << " GMT";
	return date.str();
}

std::string FileHandler::searchIndexes( const std::string& dirPath, std::vector<std::string> indexFiles ) {
	std::vector<std::string>::iterator it = indexFiles.begin();
	while (it != indexFiles.end())
	{
		std::string _fullPath = getFullPath(dirPath, *it);
		if (pathExists(_fullPath)) return _fullPath;
		++it;
	}
	return "";
}

std::string FileHandler::readFile( const std::string& filePath ) {
	std::ifstream in(filePath);
	if (!in.is_open()) return "";
	std::ostringstream sstr;
    sstr << in.rdbuf();
	in.close();
    return sstr.str();
}

FileType FileHandler::getType( const std::string& path ) {
	struct stat s;

	if( stat(path.c_str(), &s) == 0 )
	{
		if (S_ISDIR(s.st_mode)) {
			return T_DIR;
		} else if (S_ISREG(s.st_mode)) {
			return T_FILE;
		} else {
			return T_OTHER;
		}
	}
	return T_ERROR;
}

FileType FileHandler::getTypeS( const std::string& path ) {
	struct stat s;

	if( lstat(path.c_str(), &s) == 0 )
	{
		if (S_ISDIR(s.st_mode)) {
			return T_DIR;
		} else if (S_ISREG(s.st_mode)) {
			return T_FILE;
		} else if (S_ISLNK(s.st_mode)) {
			return T_LINK;
		} else {
			return T_OTHER;
		}
	}
	return T_ERROR;
}

size_t FileHandler::getFileSize(const std::string& path ) {
    struct stat st;
    if(stat(path.c_str(), &st) != 0) {
        return 0;
    }
    return st.st_size;   
}

bool FileHandler::requiresCGI( const std::map<std::string, std::string>& cgiPaths, const std::string& path ) {
	return !getCGIPath(cgiPaths, getFileExtension(path)).empty();
}

bool FileHandler::pathExists( const std::string& path ) {
	return ( access( path.c_str(), F_OK ) != -1 );
}

bool FileHandler::isPathReadable( const std::string& path ) {
	return ( access( path.c_str(), R_OK ) != -1 );
}

bool FileHandler::isPathWritable( const std::string& path ) {
	return ( access( path.c_str(), W_OK ) != -1 );
}

bool FileHandler::removeAll( const std::string& path ) {
	DIR *dir = NULL;
	struct dirent *entry;
	errno = 0;
	FileType _typeS = getTypeS(path);
	FileType _type = getType(path);

	// error while checking type.
	if (_typeS == T_ERROR) return false;

	// sym with / at end -> dir/file that doesn't exist.
	if ((_typeS == T_LINK && path.back() == '/') && !pathExists(path)) return false;

	// file | sym link with no / at end | sym link -> file
	if (_typeS == T_FILE || (_typeS == T_LINK && (path.back() != '/' || _type == T_FILE))) {
		int _ret = remove(path.c_str());
		return _ret == 0 ? true : false;
	}

	// dir | sym link with / at end -> dir
	if (_type == T_DIR && path.back() == '/') {
		dir = opendir(path.c_str());
		if (!dir) return false;
		while ((entry = readdir(dir)) != NULL) {
			if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
				continue;

			std::string _pathName = getFullPath(path, entry->d_name);

			if (getType(_pathName) == T_ERROR) {
				return false;
			}

			if (getType(_pathName) == T_DIR) {
				_pathName.push_back('/');
			}

			if (!FileHandler::removeAll(_pathName)) {
				return false;
			}
		}
	}
	int _ret = remove(path.c_str());
	closedir(dir);
	return _ret == 0 ? true : false;
}

