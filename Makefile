# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: manufern <manufern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 17:29:06 by pabad-ap          #+#    #+#              #
#    Updated: 2025/03/06 19:16:18 by manufern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-Wconversion -Wunused

INCLUDE		= -I./include/						#Directiva de los headers
HEADERS		= main.hpp Location.hpp Server.hpp Status.hpp Utils.hpp Parser.hpp Request.hpp
HEADERS		:= $(addprefix ./include/, $(HEADERS))


SRC = ./src/main.cpp \
	./src/Location.cpp \
	./src/Server.cpp \
	./src/Status.cpp \
	./src/Utils.cpp \
	./src/parser/Parser.cpp \
	./src/parser/ParserExceptions.cpp \
	./src/DeployServer/ClientRequestHandler.cpp \
	./src/DeployServer/DeployServer.cpp \
	./src/DeployServer/InitializeServer.cpp \
	./src/DeployServer/SocketPolling.cpp \
	./src/DeployServer/ConnectionHandling.cpp \
	./src/DeployServer/serverBind.cpp \
	./src/Request/cgi.cpp  \
	./src/Request/delete.cpp  \
	./src/Request/get.cpp  \
	./src/Request/httpUtils.cpp  \
	./src/Request/post.cpp  \
	./src/Request/Request.cpp
	

#Object files
OBJ_DIR	= ./build/
OBJ		= $(addprefix $(OBJ_DIR), $(notdir $(patsubst %.cpp, %.o, $(SRC))))

##Directories
DIR_SRC		= ./src/
DIR_PARSER		= $(DIR_SRC)parser/
DIR_DEPLOY		= $(DIR_SRC)DeployServer/
DIR_REQUEST		= $(DIR_SRC)Request/

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

#.cpp en carpeta SRC
$(OBJ_DIR)%.o: $(DIR_SRC)%.cpp $(HEADERS) Makefile | $(OBJ_DIR)
	$(CC) -c $(FLAGS) $(INCLUDE) $< -o $@

#.cpp en carpeta PARSER
$(OBJ_DIR)%.o: $(DIR_PARSER)%.cpp $(HEADERS) Makefile | $(OBJ_DIR)
	$(CC) -c $(FLAGS) $(INCLUDE) $< -o $@

#.cpp en carpeta DEPLOY
$(OBJ_DIR)%.o: $(DIR_DEPLOY)%.cpp $(HEADERS) Makefile | $(OBJ_DIR)
	$(CC) -c $(FLAGS) $(INCLUDE) $< -o $@

#.cpp en carpeta REQUEST
$(OBJ_DIR)%.o: $(DIR_REQUEST)%.cpp $(HEADERS) Makefile | $(OBJ_DIR)
	$(CC) -c $(FLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

do:
	@$(MAKE) > /dev/null && ./$(NAME);

profile: fclean
	@echo "Profiling result available in profile.txt"
	@$(MAKE) FLAGS="$(FLAGS) -pg" > /dev/null ;
	@./$(NAME) 2>&1 > /dev/null;
	@gprof -b $(NAME) > profile.txt;
	@rm -rf gmon.out;

valgrind:
	@$(MAKE) > /dev/null && valgrind ./$(NAME);

.PHONY: all clean fclean re do profile valgrind

