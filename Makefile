# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/31 17:16:31 by atahiri           #+#    #+#              #
#    Updated: 2022/07/19 15:29:09 by atahiri          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 	= 	webserv
CC		= 	c++
FLAGS	= 	-Wall -Wextra -Werror -std=c++98 -g
SRCS	= 	main.cpp\
			Config/Config.cpp\
			Config/ServerConfig.cpp\
			Config/Token.cpp\
			Config/Lexer.cpp\
			Config/Parser.cpp\
			Socket/Socket.cpp\
			Request/Request.cpp\


$(NAME) : 	$(SRCS)
			@$(CC) $(FLAGS) $(SRCS) -o $(NAME)

all		: 	$(NAME)
			@rm -rf webserv.dSYM

clean	:
			@rm -rf *.o

fclean	: 	clean
			@rm -rf $(NAME)

re		: 	fclean all