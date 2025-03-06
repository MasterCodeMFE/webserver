#ifndef TEST_HPP
#define TEST_HPP

// ========================================
//  INCLUDES
// ========================================
// Librerías estándar y de sistema necesarias para el servidor HTTP.
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

#include "Location.hpp"
#include "main.hpp"
#include "Parser.hpp"
#include "Server.hpp"
#include "Status.hpp"
#include "Utils.hpp"

#define PORT 8080  // 🔹 Puerto por defecto para el servidor

typedef std::set<std::pair<std::string, std::string> > listenSet;

// ========================================
//  ESTRUCTURA: HttpRequest
// ========================================
// Representa una solicitud HTTP con su método, ruta, protocolo y datos.
//
// Atributos:
// - method: Método HTTP (GET, POST, DELETE, etc.).
// - path: Ruta solicitada en el servidor.
// - protocol: Protocolo HTTP utilizado (ej. HTTP/1.1).
// - headers: Cabeceras HTTP en formato clave-valor.
// - body: Cuerpo del mensaje (para solicitudes POST).
// - query_string: Parámetros de la URL (para CGI y GET).
// ========================================
struct HttpRequest {
    std::string method;
    std::string path;
    std::string protocol;
    std::map<std::string, std::string> headers;
    std::string body;          // 🔹 Cuerpo del mensaje (para POST)
    std::string query_string;  // 🔹 Parámetros en la URL (para CGI y GET)
};

// ========================================
//  DECLARACIONES DE FUNCIONES
// ========================================

// 🔹 Funciones de creación y gestión de sockets

/*class deployServer
{
    private:
        const listenSet liseners; // lista de parejas unicas entre server name y listen
        std::vector<int> server_fds; // vector de fids de servidores
        std::vector<Location> locations; //listas de rutas

    public:
}*/



int create_socket();
void close_client(int client_fd);
int initialize_server_sockets( const listenSet &config );
int configure_and_bind_socket(int server_fd, std::pair<std::string, std::string> const &config);
int run_server_event_loop(const std::vector<int>& server_fds, const listenSet &config );
int accept_client_connection(int server_fd, const listenSet &config );

// 🔹 Manejo de solicitudes HTTP
int handle_client_request(int client_fd, const listenSet &config );
int dispatch_http_request(int client_fd, const HttpRequest& httpRequest, const listenSet &config );
std::string handle_cgi(const std::string &script_path, const std::string &query_string);
std::string handle_delete(const HttpRequest& httpRequest);
std::string handle_post(const HttpRequest& httpRequest);
std::string handle_get(const HttpRequest& request, const listenSet &config );

// 🔹 Utilidades para el manejo de archivos y respuestas HTTP
std::string get_file_path(const std::string& request_path);
std::string listDirectory(const std::string &dirPath, const std::string &requestPath);
ssize_t send_all(int sockfd, const char* buffer, size_t length);
std::string build_http_response(const std::string& content, const std::string& content_type, int status_code = 200);
std::string get_content_type(const std::string& filepath);
std::string int_to_string(int number);
std::string read_file(const std::string& filepath);

#endif // TEST_HPP
