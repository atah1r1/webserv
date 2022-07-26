/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 04:36:47 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/26 19:17:24 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __GENERAL_UTILS_HPP__
# define __GENERAL_UTILS_HPP__

#include <iostream>
#include <sstream>
#include <utility>
#include <sys/stat.h>
#include "../Config/ServerConfig.hpp"
#include "Debug.hpp"

const std::string day_names[7] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

const std::string month_names[12] = {
	"Jan",
	"Feb", 
	"Mar", 
	"Apr", 
	"May", 
	"Jun",
	"Jul", 
	"Aug", 
	"Sep", 
	"Oct", 
	"Nov", 
	"Dec"
};


bool beginsWith(const std::string& haystack, const std::string& needle);
std::pair<size_t, std::string> nextLine(const std::string& str, size_t start);
Location *matchLocation( std::vector<Location *> locations, const std::string& path);

std::string	getLastModificationDate(struct stat buffer);
std::string	getCurrentDate(void);
std::string toHeaderCase(const std::string& header);
std::string getFileExtension(const std::string& fileName);
std::string trim(const std::string& str);

template<typename T>
T toNumber(const std::string& str) {
	std::stringstream ss;
	T i = 0;

	ss << str;
	ss >> i;
	return i;
}

#endif // __GENERAL_UTILS_HPP__
