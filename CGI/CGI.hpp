/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:38:28 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/04 17:28:30 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CGI_HPP__
#define __CGI_HPP__

#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>
// #include "../Utils/Utils.hpp"

class CGI
{
public:
	static char **generateExecveArgs(const std::string &cgiPath, const std::string &filePath);
	static std::string execute(const std::string &cgiPath, const std::string &filePath, char **env);
};


#endif