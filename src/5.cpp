 // 5️⃣ Leer datos del cliente y responder al cliente

#include "test.hpp"

// Función para cerrar el socket del cliente
void close_client(int client_fd) {
    close(client_fd);
}

// Función para recibir la solicitud completa (al menos hasta los encabezados)
std::string receive_request(int client_fd) {
    std::vector<char> buffer(4096);
    ssize_t bytes_received = recv(client_fd, &buffer[0], buffer.size() - 1, 0);

    if (bytes_received == 0) {
        std::cout << "Cliente cerró la pestaña o desconectó.\n";
        close_client(client_fd);
        return "";
    } else if (bytes_received < 0) {
        std::cerr << "Error en recv().\n";
        return "";
    }

    buffer[bytes_received] = '\0';
    return std::string(&buffer[0], bytes_received);
}

// Función que analiza la solicitud HTTP y separa método, ruta, protocolo y encabezados
HttpRequest parse_request(const std::string& request) {
    HttpRequest httpRequest;
    std::istringstream stream(request);
    std::string line;

    // Extraer la primera línea: método, ruta y protocolo
    if (std::getline(stream, line)) {
        std::istringstream first_line(line);
        first_line >> httpRequest.method >> httpRequest.path >> httpRequest.protocol;
    }

    // Extraer los encabezados
    while (std::getline(stream, line) && line != "\r") {
        std::size_t pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2);
            httpRequest.headers[key] = value;
        }
    }

    return httpRequest;
}

// Función para imprimir información de depuración de la solicitud HTTP
void debug_print_http_request(const HttpRequest& httpRequest) {
    std::cout << "Método: " << httpRequest.method << "\n";
    std::cout << "Ruta: " << httpRequest.path << "\n";
    std::cout << "Protocolo: " << httpRequest.protocol << "\n";

    std::cout << "Encabezados:\n";
    // Iteración utilizando iteradores explícitos (compatible con C++98)
    for (std::map<std::string, std::string>::const_iterator it = httpRequest.headers.begin();
         it != httpRequest.headers.end(); ++it) {
        std::cout << it->first << ": " << it->second << "\n";
    }

    if (!httpRequest.body.empty()) {
        std::cout << "Cuerpo:\n" << httpRequest.body << "\n";
    }
}

// Función para leer el cuerpo HTTP si se especifica "Content-Length"
// Utiliza la parte ya recibida en raw_request y, de ser necesario, lee el resto del cuerpo.
std::string read_http_body(int client_fd, const std::string& raw_request, int content_length) {
    std::size_t pos = raw_request.find("\r\n\r\n");
    std::string body;
    if (pos != std::string::npos) {
        body = raw_request.substr(pos + 4);
    }

    int total_read = body.size();
    while (total_read < content_length) {
        std::vector<char> body_buffer(content_length - total_read);
        int bytes = recv(client_fd, &body_buffer[0], content_length - total_read, 0);
        if (bytes <= 0)
            break;
        body.append(&body_buffer[0], bytes);
        total_read += bytes;
    }

    return body;
}

// Función principal (paso_cinco) refactorizada para delegar tareas a funciones auxiliares.
int paso_cinco(int client_fd, const Config& config) {
    // Recibir solicitud inicial
    std::string raw_request = receive_request(client_fd);
    if (raw_request.empty())
        return -1;

    // Parsear la solicitud HTTP
    HttpRequest httpRequest = parse_request(raw_request);
    debug_print_http_request(httpRequest);

    // Verificar si existe el encabezado "Content-Length" para leer el cuerpo de la solicitud
    std::map<std::string, std::string>::const_iterator it = httpRequest.headers.find("Content-Length");
    if (it != httpRequest.headers.end()) {
        int content_length = 0;
        std::istringstream iss(it->second);
        iss >> content_length;
        std::cout << "Content-Length encontrado: " << content_length << "\n";

        httpRequest.body = read_http_body(client_fd, raw_request, content_length);
        if (!httpRequest.body.empty()) {
            std::cout << "Cuerpo:\n" << httpRequest.body << "\n";
        }
    }

    // Pasar la solicitud completa a la siguiente etapa (paso_seis)
    int res = paso_seis(client_fd, httpRequest, config);
    return res;
}

