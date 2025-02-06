/* 
    1. Crear el socket	Crear un socket TCP que permita conexiones.	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    2. Configurar opciones	Configurar las opciones del socket, como permitir reutilizar la dirección del puerto.	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    3. Configurar dirección	Establecer la estructura de dirección que especifica la familia de direcciones y el puerto.	address.sin_family = AF_INET; address.sin_addr.s_addr = INADDR_ANY; address.sin_port = htons(PORT);
    4. Vincular el socket	Vincular el socket a la dirección y puerto especificados.	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    5. Escuchar conexiones	Poner el socket en modo de escucha para aceptar conexiones entrantes.	listen(server_fd, 3);
    6. Aceptar conexiones	Aceptar una nueva conexión y crear un nuevo socket para el cliente.	new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    7. Leer la solicitud	Leer los datos enviados por el cliente (solicitud HTTP).	read(new_socket, buffer, 1024);
    8. Procesar la solicitud	Analizar la solicitud (por ejemplo, extraer el método y la ruta) y determinar la respuesta.	std::cout << "Solicitud recibida:\n" << buffer << std::endl;
    9. Enviar la respuesta	Formar y enviar una respuesta HTTP adecuada al cliente.	send(new_socket, response, strlen(response), 0);
    10. Cerrar la conexión	Cerrar el socket del cliente después de enviar la respuesta.	close(new_socket);
    11. Repetir	Volver al paso 6 para aceptar nuevas conexiones.	while (true) { ... }
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <string>

#define PORT 8080

void send_file(int socket, const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;

        // Abrir la página de error 404
        std::ifstream error_file("./error/404.html");
        std::string error_content;
        if (error_file.is_open()) {
            error_content.assign((std::istreambuf_iterator<char>(error_file)),
                                  std::istreambuf_iterator<char>());
            error_file.close();
        } else {
            error_content = "<html><body><h1>404 Not Found</h1><p>Página no encontrada.</p>"
                            "<a href=\"/index.html\">Volver al inicio</a></body></html>";
        }

        // Construir la respuesta HTTP con la página de error
        std::string error_response = "HTTP/1.1 404 Not Found\r\n"
                                     "Content-Type: text/html\r\n"
                                     "Content-Length: " + std::to_string(error_content.size()) + "\r\n"
                                     "\r\n" +
                                     error_content;

        send(socket, error_response.c_str(), error_response.size(), 0);
        return;
    }

    // Leer el contenido del archivo
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();

    // Construir la respuesta HTTP con el contenido del archivo
    std::string response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: " + std::to_string(content.size()) + "\r\n"
                           "\r\n" +
                           content;

    // Enviar la respuesta al cliente
    send(socket, response.c_str(), response.size(), 0);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Crear el socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error en socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Forzar la vinculación del socket al puerto
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error en setsockopt: " << strerror(errno) << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Configurar la estructura de la dirección
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Cualquier dirección
    address.sin_port = htons(PORT); // Puerto

    // Vincular el socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error en bind: " << strerror(errno) << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Escuchar conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error en listen: " << strerror(errno) << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor escuchando en el puerto " << PORT << std::endl;

    // Bucle principal para aceptar y manejar conexiones
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            std::cerr << "Error en accept: " << strerror(errno) << std::endl;
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Leer la solicitud del cliente
        char buffer[1024] = {0};
        ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            std::cerr << "Error en read: " << strerror(errno) << std::endl;
        } else {
            buffer[bytes_read] = '\0'; // Asegurarse de que la cadena esté terminada
            std::cout << "Solicitud recibida:\n" << buffer << std::endl;

            // Parsear la solicitud para extraer el path
            std::istringstream request_stream(buffer);
            std::string method, request_path, http_version;
            request_stream >> method >> request_path >> http_version;
            
            // Determinar qué archivo servir según el path
            std::string file_to_send;
            if (request_path == "/" || request_path == "/index.html") {
                file_to_send = "index.html"; // Asegúrate de tenerlo en la raíz
            } else if (request_path.find("/error") == 0) {
                // Si la solicitud es para algo dentro de /error, quitamos la primera barra
                file_to_send = request_path.substr(1);
            } else {
                // Para cualquier otro path, asumiendo que el archivo está en la raíz
                file_to_send = request_path.substr(1); // Quita la '/' inicial
            }

            // Llama a send_file con el archivo adecuado
            send_file(new_socket, file_to_send);
        }

        // Cerrar el socket del cliente
        close(new_socket);
    }

    // Cerrar el socket del servidor (aunque en un servidor real esto no se alcanzaría)
    close(server_fd);
    return 0;
}