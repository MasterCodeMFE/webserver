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

int paso_cinco(int client_fd)
{
    std::vector<char> buffer(1024);
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
    HttpRequest httpRequest = parse_request(request);

    int res = paso_seis(client_fd, httpRequest);
    
    return res;
}
