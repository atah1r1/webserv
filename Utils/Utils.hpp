/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 04:36:47 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/02 17:45:47 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __GENERAL_UTILS_HPP__
#define __GENERAL_UTILS_HPP__

#include <iostream>
#include <sstream>
#include <utility>
#include <sys/stat.h>
#include <unistd.h>
#include <iomanip>
#include <map>
#include "../Config/ServerConfig.hpp"
#include "../Includes/Webserv.hpp"
#include "Debug.hpp"

const std::string day_names[7] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"};

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
	"Dec"};

bool beginsWith(const std::string &haystack, const std::string &needle);
std::pair<size_t, std::string> nextLine(const std::string &str, size_t start);
Location *matchLocation(std::vector<Location *> locations, const std::string &path);

std::string getCurrentDate(void);
std::string toHeaderCase(const std::string &header);
std::string getMimeType(const std::string &extension);
std::string getCGIPath(const std::string &extension);
std::string trim(const std::string &str);
std::string toUpperCase(const std::string &str);
std::string toLowerCase(const std::string &str);

bool isMethodAllowed(std::vector<std::string> allowedMethods, const std::string &method);
bool isMethodImplemented(const std::string &method);

template <typename T>
T toNumber(const std::string &str)
{
	std::stringstream ss;
	T i = 0;

	ss << str;
	ss >> i;
	return i;
}

template <typename T>
std::string toString(T t)
{
	std::stringstream ss;

	ss << t;
	return ss.str();
}

#endif // __GENERAL_UTILS_HPP__
