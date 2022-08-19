/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:38:28 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/19 19:58:44 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CGI_HPP__
#define __CGI_HPP__

#include "../Utils/Utils.hpp"
#include <vector>

class CGI
{
public:
	static char **generateExecveArgs(const std::string &cgiPath, const std::string &filePath);
	static std::string execute(const std::string &cgiPath, const std::string &filePath, const std::string& inputPath, char *const* env );
};

#endif