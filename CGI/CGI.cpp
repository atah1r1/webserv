/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 12:35:16 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/19 23:48:12 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

char **CGI::generateExecveArgs(const std::string &cgiPath, const std::string &filePath)
{
	char **args = (char **)malloc(sizeof(char *) * 3);
	args[0] = strdup(cgiPath.c_str());
	args[1] = strdup(filePath.c_str());
	args[2] = NULL;
	return args;
}

std::string CGI::execute(const std::string &cgiPath, const std::string &filePath, const std::string& inputPath, char *const* env )
{
	int status;
	std::string outFilePath = randomFileName();

	pid_t pid = fork();

	if (pid == -1)
	{
		std::cerr << "Error: fork() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		int ffd = open(outFilePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (ffd < 0)
		{
			std::cerr << "Error: open() failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		dup2(ffd, STDOUT_FILENO);
		if (!inputPath.empty())
		{
			int infd = open(inputPath.c_str(), O_RDONLY, 0644);
			if (infd != -1)
				dup2(infd, STDIN_FILENO);
		}
		char **args = CGI::generateExecveArgs(cgiPath, filePath);
		execve(args[0], args, env);

		std::cerr << "Error: execve() failed." << strerror(errno) << std::endl;
		close(ffd);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) == 1)
			return "";
	}
	return outFilePath;
}