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

#define PORT 8080
#ifndef MAXCLIENT
#define MAXCLIENT 1
#endif

struct HttpRequest
{
    std::string method;
    std::string path;
    std::string protocol;
    std::map<std::string, std::string> headers;
};


int create_socket();
int paso_dos(int server_fd);
int paso_tres(int server_fd);
int paso_cuatro(int server_fd);
int paso_cinco(int client_fd);
int paso_seis(int client_fd, const HttpRequest& httpRequest);

#endif // TEST_HPP