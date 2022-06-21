/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 12:35:16 by aes-salm          #+#    #+#             */
/*   Updated: 2022/06/21 01:32:22 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

char **generate_execve_args(std::string path)
{
	char **execve_args = (char **)malloc(sizeof(char *) * 3);
	execve_args[0] = (char *)"./bin/PHP_CGI";
	execve_args[1] = strdup(path.c_str());
	execve_args[2] = nullptr;
	return execve_args;
}

void cgi(std::string file_path, char **envp)
{
	pid_t pid = fork();
	int status;
	int fd[2];
	pipe(fd);

	char output[1024];
	memset(output, 0, 1024);
	// int fd = open("./file.txt", O_WRONLY | O_CREAT);

	if (pid == -1)
	{
		std::cout << "Error: fork() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(fd[0]);
		write(fd[1], "Hello World!", strlen("Hello World!"));
		// dup2(fd[1], 1);
		// char **execve_args = generate_execve_args(file_path);
		// execve(execve_args[0], execve_args, envp);
	}
	else
	{
		waitpid(pid, &status, 0);
		close(fd[1]);
		read(fd[0], output, 1024);
		close(fd[0]);
		std::cout << output << std::endl;
	}
}

int main(int argc, char **argv, char **envp)
{
	cgi("./tests/test.php", envp);
	return (0);
}