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
