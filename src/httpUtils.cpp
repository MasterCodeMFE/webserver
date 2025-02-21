#include "test.hpp"

// Lee un archivo y devuelve su contenido.
std::string read_file(const std::string& filepath)
{
    std::ifstream file(filepath.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Convierte un entero a std::string (compatible con C++98).
std::string int_to_string(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

// Devuelve el tipo de contenido basado en la extensión del archivo.
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
        if (mime_types.find(ext) != mime_types.end())
        {
            return mime_types[ext];
        }
    }
    return "application/octet-stream"; // Tipo por defecto
}

// Construye una respuesta HTTP a partir del contenido, tipo y código de estado.
std::string build_http_response(const std::string& content, const std::string& content_type, int status_code)
{
    std::string status_text;
    switch (status_code)
    {
        case 200: status_text = "OK"; break;
        case 400: status_text = "Bad Request"; break;
        case 403: status_text = "Forbidden"; break;
        case 404: status_text = "Not Found"; break;
        case 405: status_text = "Method Not Allowed"; break;
        case 500: status_text = "Internal Server Error"; break;
        default: status_text = "OK"; break;
    }
    return "HTTP/1.1 " + int_to_string(status_code) + " " + status_text + "\r\n"
           "Content-Type: " + content_type + "\r\n"
           "Content-Length: " + int_to_string(content.size()) + "\r\n\r\n" + content;
}

// Envía todos los datos a través del socket.
ssize_t send_all(int sockfd, const char* buffer, size_t length)
{
    size_t total_sent = 0;
    while (total_sent < length)
    {
        ssize_t bytes_sent = send(sockfd, buffer + total_sent, length - total_sent, 0);
        if (bytes_sent <= 0)
        {
            std::cerr << "❌ Error enviando datos al cliente." << std::endl;
            return -1;
        }
        total_sent += bytes_sent;
    }
    return total_sent;
}