#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <poll.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <filesystem>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <sys/stat.h>
#include <dirent.h>
#include <netdb.h>
#include "./../templates/Config.hpp"

#define PORT 8080


struct HttpRequest
{
    std::string method;
    std::string path;
    std::string protocol;
    std::map<std::string, std::string> headers;
    std::string body;  // 🔹 Cuerpo del mensaje (para POST)
    std::string query_string;  // 🔹 Parámetros en la URL (para CGI y GET)
};

int create_socket();
void close_client(int client_fd);
int paso_uno(Config const &config);
int paso_dos(int server_fd, Config const &config, int i);
int paso_tres(const std::vector<int>& server_fds, const Config& config);
int paso_cuatro(int server_fd, Config const &config);
int paso_cinco(int client_fd, Config const &config);
int paso_seis(int client_fd, const HttpRequest& httpRequest, Config const &config);

#endif // TEST_HPP