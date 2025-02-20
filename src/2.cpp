// 2️⃣ Configurar la dirección del servidor y Vincular el socket

#include "test.hpp"

// Función auxiliar para convertir una cadena a entero.
int string_to_int(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    return value;
}

// Función para analizar la cadena "listen" y extraer la IP y el puerto.
std::pair<std::string, std::string> parseListenString(const std::string& listen_str) {
    std::pair<std::string, std::string> result;
    size_t pos = listen_str.find(':');
    if (pos != std::string::npos) {
        result.first = listen_str.substr(0, pos);
        result.second = listen_str.substr(pos + 1);
    } else {
        // Si no se encuentra “:”, se asume que solo se especifica el puerto.
        result.first = "";
        result.second = listen_str;
    }
    return result;
}

// Función para resolver la dirección utilizando getaddrinfo.
struct addrinfo* getResolvedAddress(const std::string& ip, const std::string& port_str) {
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Usar INADDR_ANY si la IP está vacía

    const char* ip_cstr = ip.empty() ? NULL : ip.c_str();
    struct addrinfo *res;
    int status = getaddrinfo(ip_cstr, port_str.c_str(), &hints, &res);
    if (status != 0) {
        std::cerr << "Error en getaddrinfo con la IP: " 
                  << (ip.empty() ? "INADDR_ANY" : ip) << std::endl;
        return NULL;
    }
    return res;
}

// Función para vincular el socket y mostrar mensajes de estado.
int attemptBind(int server_fd, struct addrinfo* res, const std::string& ip, int port) {
    std::cout << "Intentando vincular el socket en " 
              << (ip.empty() ? "INADDR_ANY" : ip)
              << ":" << port << std::endl;

    if (bind(server_fd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "Error al vincular el socket en " 
                  << (ip.empty() ? "INADDR_ANY" : ip)
                  << ":" << port << ": " << strerror(errno) << std::endl;
        freeaddrinfo(res);
        return -1;
    }
    freeaddrinfo(res);
    std::cout << "Se ha vinculado correctamente en " 
              << (ip.empty() ? "INADDR_ANY" : ip)
              << ":" << port << std::endl;
    return 0;
}

// Función principal refactorizada que utiliza las funciones anteriores.
int paso_dos(int server_fd, Config const &config, int i) {
    // Obtener la cadena "listen" del servidor.
    std::string listen_str = config.getVServers()[i]->getVListen()[0];
    
    // Analizar la cadena para extraer la IP y el puerto.
    std::pair<std::string, std::string> listenPair = parseListenString(listen_str);
    std::string ip = listenPair.first;
    std::string port_str = listenPair.second;
    int port = string_to_int(port_str);

    // Resolver la dirección.
    struct addrinfo* res = getResolvedAddress(ip, port_str);
    if (!res) {
        return -1;
    }

    // Intentar vincular el socket.
    return attemptBind(server_fd, res, ip, port);
}