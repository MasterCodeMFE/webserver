# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 17:29:06 by pabad-ap          #+#    #+#              #
#    Updated: 2025/02/13 17:29:09 by pabad-ap         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------------    PABLO    ------------
#
#NAME		= webserv
#
#CPP			= c++
#CPP_FLAGS	= -Wall -Wextra -Werror -std=c++98 #-Wconversion -Wunused
#
#INCLUDE		= -I./include/						#Directiva de los headers
#HEADERS		= main.hpp Location.hpp Server.hpp Status.hpp Utils.hpp Parser.hpp
#HEADERS		:= $(addprefix ./include/, $(HEADERS))
#
#
#SRCS		= ./src/Location.cpp \
#			./src/Server.cpp \
#			./src/Status.cpp \
#			./src/Utils.cpp \
#			./src/parser/Parser.cpp \
#			./src/parser/ParserExceptions.cpp \
#			./src/parser/main.cpp
##			./templates/mainNewConfig.cpp
##			./templates/mainTestConfig.cpp
#
##Object files
#DIR_OBJS	= ./build/
#OBJS		= $(addprefix $(DIR_OBJS), $(notdir $(patsubst %.cpp, %.o, $(SRCS))))
#
##Directories
#DIR_SRC			= ./src/
#DIR_TEMPLATES	= ./templates/
#DIR_PARSER		= $(DIR_SRC)parser/
#
#all: $(NAME)
#
#$(NAME): $(OBJS)
#	$(CPP) $(CPP_FLAGS) $(OBJS) -o $(NAME)
#
## Creación de directorio objectos si no existen
#$(DIR_OBJS):
#	mkdir $(DIR_OBJS)
#
##.cpp en carpeta TEMPLATES
#$(DIR_OBJS)%.o: $(DIR_TEMPLATES)%.cpp $(HEADERS) Makefile | $(DIR_OBJS)
#	$(CPP) -c $(CPP_FLAGS) $(INCLUDE) $< -o $@
#
##.cpp en carpeta SRC
#$(DIR_OBJS)%.o: $(DIR_SRC)%.cpp $(HEADERS) Makefile | $(DIR_OBJS)
#	$(CPP) -c $(CPP_FLAGS) $(INCLUDE) $< -o $@
#
##.cpp en carpeta PARSER
#$(DIR_OBJS)%.o: $(DIR_PARSER)%.cpp $(HEADERS) Makefile | $(DIR_OBJS)
#	$(CPP) -c $(CPP_FLAGS) $(INCLUDE) $< -o $@
#
#
#clean:
#	rm -rf $(DIR_OBJS)
#	
#fclean: clean
#	rm -rf $(NAME)
#
#re: fclean all
#
#do:
#	@$(MAKE) > /dev/null && ./$(NAME);
#
#profile: fclean
#	@echo "Profiling result available in profile.txt"
#	@$(MAKE) CPP_FLAGS="$(CPP_FLAGS) -pg" > /dev/null ;
#	@./$(NAME) 2>&1 > /dev/null;
#	@gprof -b $(NAME) > profile.txt;
#	@rm -rf gmon.out;
#
#valgrind:
#	@$(MAKE) > /dev/null && valgrind ./$(NAME);
#
#.PHONY: all clean fclean re do profile valgrind


# ------------    MANU    ------------
NAME = webserv

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98

SRC = ./src/1serverSocketInit.cpp \
      ./src/2serverBind.cpp \
      ./src/3socketPolling.cpp \
      ./src/4connectionHandling.cpp \
      ./src/5clientRequestHandler.cpp \
      ./src/6httpRequestDispatcher.cpp \
      ./templates/mainTestConfig.cpp \
      ./templates/Config.cpp \
      ./templates/Location.cpp \
      ./templates/Server.cpp \
      ./templates/Status.cpp \
      ./templates/Utils.cpp \
      ./src/cgi.cpp \
      ./src/delete.cpp \
      ./src/get.cpp \
      ./src/post.cpp \
      ./src/fileManager.cpp \
      ./src/httpUtils.cpp

OBJ_DIR = obj
OBJ = $(patsubst ./src/%.cpp, $(OBJ_DIR)/%.o, $(filter ./src/%.cpp, $(SRC)))
OBJ += $(patsubst ./templates/%.cpp, $(OBJ_DIR)/%.o, $(filter ./templates/%.cpp, $(SRC)))

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: ./src/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: ./templates/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
