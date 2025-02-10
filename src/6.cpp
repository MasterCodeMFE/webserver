#include "test.hpp"


// 📌 Función para leer un archivo y devolver su contenido
std::string read_file(const std::string& filepath)
{
    std::ifstream file(filepath.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

// 📌 Función auxiliar para convertir `int` a `std::string` en C++98
std::string int_to_string(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

// 📌 Obtiene el tipo de contenido según la extensión del archivo
std::string get_content_type(const std::string& filepath)
{
    std::map<std::string, std::string> mime_types;
    mime_types[".html"] = "text/html";
    mime_types[".css"] = "text/css";
    mime_types[".js"] = "application/javascript";
    mime_types[".png"] = "image/png";
    mime_types[".jpg"] = "image/jpeg";
    mime_types[".jpeg"] = "image/jpeg";
    mime_types[".gif"] = "image/gif";
    mime_types[".txt"] = "text/plain";

    size_t dot_pos = filepath.find_last_of(".");
    if (dot_pos != std::string::npos)
    {
        std::string ext = filepath.substr(dot_pos);
        std::map<std::string, std::string>::iterator it = mime_types.find(ext);
        if (it != mime_types.end())
        {
            return it->second;
        }
    }
    return "application/octet-stream"; // Tipo por defecto
}


// 📌 Función que maneja las solicitudes GET
std::string handle_get(const HttpRequest& request)
{
    std::string filepath = "www" + request.path;
    if (request.path == "/")
    {
        filepath = "www/index.html";
    }

    std::string content = read_file(filepath);
    if (content.empty())
    {
        return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nPagina no existe\n";
    }

    std::string content_type = get_content_type(filepath);
    return "HTTP/1.1 200 OK\r\nContent-Type: " + content_type + "\r\nContent-Length: " +
           int_to_string(content.size()) + "\r\n\r\n" + content;
}

// 📌 Envía todos los datos correctamente
ssize_t send_all(int sockfd, const char* buffer, size_t length)
{
    size_t total_sent = 0;
    ssize_t bytes_sent;

    while (total_sent < length)
    {
        bytes_sent = send(sockfd, buffer + total_sent, length - total_sent, 0);
        if (bytes_sent <= 0)
        {
            std::cerr << "❌ Error enviando datos al cliente." << std::endl;
            return -1;
        }
        total_sent += bytes_sent;
    }
    return total_sent;
}

// 📌 Nueva función paso_seis
int paso_seis(int client_fd, const HttpRequest& httpRequest)
{
    std::string response;

    if (httpRequest.method == "GET")
    {
        response = handle_get(httpRequest);
    }
    else
    {
        response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\nContent-Length: 24\r\n\r\nMetodo no permitido\n";
    }

    std::cout << "\U0001F4E4 Enviando respuesta al cliente:\n" << response << "\n";
    send_all(client_fd, response.c_str(), response.size());

    return 0;  // 🔵 Mantener la conexión abierta tras responder
}
