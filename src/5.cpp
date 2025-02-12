 // 5️⃣ Leer datos del cliente y responder al cliente

#include "test.hpp"

void close_client(int client_fd)
{
    close(client_fd);
}

HttpRequest parse_request(const std::string& request)
{
    HttpRequest httpRequest;
    std::istringstream stream(request);
    std::string line;

    // Obtener la primera línea (método, ruta y protocolo)
    if (std::getline(stream, line))
    {
        std::istringstream first_line(line);
        first_line >> httpRequest.method >> httpRequest.path >> httpRequest.protocol;
    }

    // Obtener los encabezados
    while (std::getline(stream, line) && line != "\r")
    {
        std::size_t pos = line.find(": ");
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2);
            httpRequest.headers[key] = value;
        }
    }

    return httpRequest;
}

int paso_cinco(int client_fd, Config const &config)
{
    (void)config;
    std::vector<char> buffer(4096);  // 🔹 Aumentar el buffer para capturar encabezados y cuerpo
    ssize_t bytes_received = recv(client_fd, &buffer[0], buffer.size() - 1, 0);

    if (bytes_received == 0)
    {
        std::cout << "Cliente cerró la pestaña o desconectó.\n";
        close(client_fd);
        return -1;
    }
    else if (bytes_received < 0)
    {
        std::cerr << "Error en recv().\n";
        return -1;
    }

    buffer[bytes_received] = '\0';
    std::string request(&buffer[0], bytes_received);
    
    // 🔹 Parseamos la solicitud HTTP
    HttpRequest httpRequest = parse_request(request);

    std::cout << "Método: " << httpRequest.method << "\n";
    std::cout << "Ruta: " << httpRequest.path << "\n";
    std::cout << "Protocolo: " << httpRequest.protocol << "\n";

    std::cout << "Encabezados:\n";
    for (std::map<std::string, std::string>::const_iterator it = httpRequest.headers.begin(); it != httpRequest.headers.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << "\n";
    }

    // Usamos una nueva variable para la búsqueda de "Content-Length"
    std::map<std::string, std::string>::const_iterator it_length = httpRequest.headers.find("Content-Length");
    if (it_length != httpRequest.headers.end())
    {
        std::cout << "Content-Length encontrado: " << it_length->second << "\n";
    }

    if (!httpRequest.body.empty())
    {
        std::cout << "Cuerpo:\n" << httpRequest.body << "\n";
    }





    // 🔹 Verificamos si hay un cuerpo en la solicitud (Content-Length)
    std::map<std::string, std::string>::iterator it = httpRequest.headers.find("Content-Length");
    if (it != httpRequest.headers.end())
    {
        int content_length = 0;
        std::istringstream(it->second) >> content_length;
        std::string body;
        body.reserve(content_length);

        // Si el cuerpo no está completamente en el primer recv(), leemos el resto
        int total_read = request.substr(request.find("\r\n\r\n") + 4).size();  // Ya leído
        body = request.substr(request.find("\r\n\r\n") + 4); // Extraer datos ya recibidos

        while (total_read < content_length)
        {
            std::vector<char> body_buffer(content_length - total_read);
            int bytes = recv(client_fd, &body_buffer[0], content_length - total_read, 0);
            if (bytes <= 0) break;
            body.append(&body_buffer[0], bytes);
            total_read += bytes;
        }

        httpRequest.body = body;
    }

    // 🔹 Pasamos la solicitud completa a `paso_seis`
    int res = paso_seis(client_fd, httpRequest, config);
    
    return res;
}
