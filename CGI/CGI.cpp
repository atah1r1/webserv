/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 12:35:16 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/05 11:58:09 by aes-salm         ###   ########.fr       */
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

std::string CGI::execute(const std::string &cgiPath, const std::string &filePath, char **env)
{
	int status;
	int fd[2];
	struct stat sb;
	std::string result;

	pipe(fd);
	pid_t pid = fork();

	if (pid == -1)
	{
		std::cout << "Error: fork() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		char **args = CGI::generateExecveArgs(cgiPath, filePath);
		execve(args[0], args, env);
	}
	else
	{
		waitpid(pid, &status, 0);
		close(fd[1]);

		fstat(fd[0], &sb);		   // get file size
		result.resize(sb.st_size); // resize string to file size

		read(fd[0], (char *)(result.data()), sb.st_size);
		close(fd[0]);
	}
	return result;
}