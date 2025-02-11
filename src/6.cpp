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

std::string extract_filename(const std::string& body)
{
    std::string header_start = "filename=\"";
    size_t filename_pos = body.find(header_start);
    if (filename_pos == std::string::npos)
        return "archivo_subido"; // Si no se encuentra, usa un nombre por defecto

    filename_pos += header_start.length();
    size_t filename_end = body.find("\"", filename_pos);
    if (filename_end == std::string::npos)
        return "archivo_subido";

    return body.substr(filename_pos, filename_end - filename_pos);
}

std::string save_uploaded_file(const std::string& body, const std::string& boundary)
{
    // Extraer el nombre del archivo
    std::string filename = extract_filename(body);

    // Buscar la primera ocurrencia del contenido del archivo
    size_t file_start = body.find("\r\n\r\n") + 4;
    size_t file_end = body.rfind("--" + boundary) - 2; // Excluir el "--"

    if (file_start == std::string::npos || file_end == std::string::npos || file_end <= file_start)
    {
        return "Error: No se pudo encontrar el archivo en la solicitud.";
    }

    std::string file_content = body.substr(file_start, file_end - file_start);
    std::string file_path = "upload/" + filename; // Usa el nombre original

    std::ofstream outfile(file_path.c_str(), std::ios::binary);
    if (!outfile)
    {
        return "Error: No se pudo guardar el archivo.";
    }

    outfile.write(file_content.c_str(), file_content.size());
    outfile.close();

    return "Archivo guardado en: " + file_path;
}

std::string handle_post(const HttpRequest& httpRequest)
{
    std::map<std::string, std::string>::const_iterator it = httpRequest.headers.find("Content-Type");
    if (it == httpRequest.headers.end())
    {
        return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 25\r\nConnection: close\r\n\r\nFalta Content-Type\n";
    }

    std::string content_type = it->second;
    std::string response_body;

    std::cout << "📥 Datos recibidos en el POST:\n" << httpRequest.body << "\n"; // 🔹 Mostrar en consola

    if (content_type.find("multipart/form-data") != std::string::npos)
    {
        // 🔹 Extraer el boundary
        size_t pos = content_type.find("boundary=");
        if (pos == std::string::npos)
        {
            return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 32\r\nConnection: close\r\n\r\nFalta el boundary en Content-Type\n";
        }
        std::string boundary = content_type.substr(pos + 9);
        response_body = save_uploaded_file(httpRequest.body, boundary);
    }
    else
    {
        response_body = "POST recibido con éxito\nDatos:\n" + httpRequest.body;
    }

    std::ostringstream oss;
    oss << response_body.size();
    std::string content_length = oss.str();

    return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + content_length + "\r\nConnection: keep-alive\r\n\r\n" + response_body;
}

std::string handle_delete(const HttpRequest& httpRequest)
{
    std::string filepath = "www" + httpRequest.path;

    // Verificar si el archivo existe
    if (access(filepath.c_str(), F_OK) != 0)
    {
        return "HTTP/1.1 404 Not Found\r\n"
               "Content-Type: text/plain\r\n"
               "Content-Length: 16\r\n\r\n"
               "Archivo no existe\n";
    }

    // Intentar eliminar el archivo
    if (remove(filepath.c_str()) != 0)
    {
        return "HTTP/1.1 500 Internal Server Error\r\n"
               "Content-Type: text/plain\r\n"
               "Content-Length: 30\r\n\r\n"
               "Error al eliminar el archivo\n";
    }

    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: 25\r\n\r\n"
           "Archivo eliminado con éxito\n";
}

// 📌 Modificar paso_seis para incluir DELETE
int paso_seis(int client_fd, const HttpRequest& httpRequest)
{
    std::string response;

    if (httpRequest.method == "GET")
    {
        response = handle_get(httpRequest);
    }
    else if (httpRequest.method == "POST")
    {
        response = handle_post(httpRequest);
    }
    else if (httpRequest.method == "DELETE")
    {
        response = handle_delete(httpRequest);
    }
    else
    {
        response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\nContent-Length: 24\r\n\r\nMetodo no permitido\n";
    }

    std::cout << "\U0001F4E4 Enviando respuesta...\n";
    send_all(client_fd, response.c_str(), response.size()); 
    return 0;
}

