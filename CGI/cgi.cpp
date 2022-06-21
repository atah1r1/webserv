/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 12:35:16 by aes-salm          #+#    #+#             */
/*   Updated: 2022/06/21 15:19:41 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>

char **generate_execve_args(std::string path)
{
	char **execve_args = (char **)malloc(sizeof(char *) * 3);
	execve_args[0] = (char *)"./bin/PHP_CGI";
	execve_args[1] = strdup(path.c_str());
	execve_args[2] = NULL;
	return execve_args;
}

std::string cgi(std::string file_path, char **envp)
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
		char **execve_args = generate_execve_args(file_path);
		execve(execve_args[0], execve_args, envp);
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

int main(int argc, char **argv, char **envp)
{
	std::cout << cgi("./tests/test.html", envp) << std::endl;
	return (0);
}