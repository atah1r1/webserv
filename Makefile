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
      Request/Utils.cpp\
			Utils/FileHandler.cpp\
			Utils/Utils.cpp\



$(NAME) : 	$(SRCS)
			@$(CC) $(FLAGS) $(SRCS) -o $(NAME)

all		: 	$(NAME)
			@rm -rf webserv.dSYM

clean	:
			@rm -rf *.o

fclean	: 	clean
			@rm -rf $(NAME)

re		: 	fclean all