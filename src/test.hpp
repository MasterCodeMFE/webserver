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

#include <signal.h>

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
int initialize_server_sockets(Config const &config);
int configure_and_bind_socket(int server_fd, Config const &config, int i);
int run_server_event_loop(const std::vector<int>& server_fds, const Config& config);
int accept_client_connection(int server_fd, Config const &config);
int handle_client_request(int client_fd, Config const &config);
int dispatch_http_request(int client_fd, const HttpRequest& httpRequest, Config const &config);
std::string handle_cgi(const std::string &script_path, const std::string &query_string);
std::string handle_delete(const HttpRequest& httpRequest);
std::string handle_post(const HttpRequest& httpRequest);
std::string handle_get(const HttpRequest& request, const Config &config);
std::string get_file_path(const std::string& request_path);
std::string listDirectory(const std::string &dirPath, const std::string &requestPath);
ssize_t send_all(int sockfd, const char* buffer, size_t length);
std::string build_http_response(const std::string& content, const std::string& content_type, int status_code = 200);
std::string get_content_type(const std::string& filepath);
std::string int_to_string(int number);
std::string read_file(const std::string& filepath);



#endif // TEST_HPP