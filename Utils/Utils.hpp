/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 04:36:47 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/26 04:57:48 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __GENERAL_UTILS_HPP__
# define __GENERAL_UTILS_HPP__

#include <iostream>
#include <sstream>
#include <utility>
#include "../Config/ServerConfig.hpp"
#include "Debug.hpp"

const char *getFileExtension(const std::string& fileName);
bool beginsWith(const std::string& haystack, const std::string& needle);
const std::string trim(const std::string& str);
std::pair<size_t, std::string> nextLine(const std::string& str, size_t start);
Location *matchLocation( std::vector<Location *> locations, const std::string& path);

template<typename T>
T toNumber(const std::string& str) {
	std::stringstream ss;
	T i = 0;

	ss << str;
	ss >> i;
	return i;
}

#endif // __GENERAL_UTILS_HPP__
