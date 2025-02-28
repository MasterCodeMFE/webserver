#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include <algorithm>

#define PORT 8080 // Define el puerto del servidor

// Función para determinar el tipo de contenido basado en la extensión del archivo
std::string get_content_type(const std::string &filename)
{
    std::map<std::string, std::string> mime_types = {
        {".html", "text/html"},
        {".htm", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        {".png", "image/png"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".gif", "image/gif"},
        {".svg", "image/svg+xml"},
        {".txt", "text/plain"},
        {".xml", "application/xml"},
        {".pdf", "application/pdf"},
        {".zip", "application/zip"},
    };

    std::string extension = filename.substr(filename.find_last_of('.'));
    if (mime_types.find(extension) != mime_types.end())
    {
        return mime_types[extension];
    }
    return "application/octet-stream"; // Tipo por defecto
}

// Función para decodificar la ruta de la solicitud
std::string decode_url(const std::string &url)
{
    std::string decoded_url;
    for (size_t i = 0; i < url.length(); i++)
    {
        if (url[i] == '%')
        {
            // Convertir el código hexadecimal a un carácter
            if (i + 2 < url.length())
            {
                std::string hex_code = url.substr(i + 1, 2);
                char decoded_char = static_cast<char>(std::stoi(hex_code, nullptr, 16));
                decoded_url += decoded_char;
                i += 2; // Saltar los dos caracteres hexadecimales
            }
        }
        else if (url[i] == '+')
        {
            decoded_url += ' '; // Convertir '+' a espacio
        }
        else
        {
            decoded_url += url[i];
        }
    }
    return decoded_url;
}

void send_response(int socket, int status_code, const std::string &status_message, const std::string &content, const std::string &content_type)
{
    std::string response = "HTTP/1.1 " + std::to_string(status_code) + " " + status_message + "\r\n" +
                           "Content-Type: " + content_type + "\r\n" +
                           "Content-Length: " + std::to_string(content.size()) + "\r\n" +
                           "\r\n" +
                           content;
    std::cout << response; // Imprimir respuesta en la consola
    send(socket, response.c_str(), response.size(), 0);
}

void send_file(int socket, const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;

        // Abrir la página de error 404
        std::ifstream error_file("./error/404.html");
        std::string error_content;

        if (error_file.is_open())
        {
            error_content.assign((std::istreambuf_iterator<char>(error_file)),
                                 std::istreambuf_iterator<char>());
            error_file.close();
        }
        else
        {
            // Contenido alternativo para la página 404 si no se puede abrir el archivo
            error_content = "<html><body><h1>404 Not Found</h1><p>Página no encontrada.</p>"
                            "<a href=\"/index.html\">Volver al inicio</a></body></html>";
        }

        // Enviar la respuesta con el contenido de error
        send_response(socket, 404, "Not Found", error_content, "text/html");
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // Determinar el tipo de contenido basado en la extensión del archivo
    std::string content_type = get_content_type(filename);
    send_response(socket, 200, "OK", content, content_type);
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Error en socket\n";
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "Error en setsockopt\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Error en bind\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Error en listen\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor escuchando en el puerto " << PORT << std::endl;

    while (true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
        {
            std::cerr << "Error en accept\n";
            continue;
        }

        char buffer[1024] = {0};
        ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0)
        {
            send_response(new_socket, 400, "Bad Request", "<html><body><h1>400 Bad Request</h1></body></html>", "text/html");
            close(new_socket);
            continue;
        }

        buffer[bytes_read] = '\0';
        std::istringstream request_stream(buffer);
        std::string method, request_path, http_version;
        request_stream >> method >> request_path >> http_version;

        if (method != "GET")
        {
            send_response(new_socket, 405, "Method Not Allowed", "<html><body><h1>405 Method Not Allowed</h1></body></html>", "text/html");
            close(new_socket);
            continue;
        }

        if (request_path == "/")
        {
            request_path = "/index.html"; // Cambiar a index.html si solo se solicita la raíz
        }

        // Decodificar la ruta solicitada
        std::string decoded_path = decode_url(request_path.substr(1)); // Remover el primer caracter "/"
        
        // Llama a send_file para enviar el archivo solicitado
        send_file(new_socket, decoded_path);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}