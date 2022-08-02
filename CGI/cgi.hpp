/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:38:28 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/02 16:19:18 by ehakam           ###   ########.fr       */
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
#include "../Utils/Utils.hpp"

std::string cgi(const std::string& cgiPath, std::string file_path, char * const *envp);

#endif