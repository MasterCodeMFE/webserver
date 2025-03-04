// 2️⃣ Configurar la dirección del servidor y Vincular el socket

#include "test.hpp"

// ========================================
//  FUNCIÓN: string_to_int
// ========================================
// Convierte una cadena de caracteres en un número entero.
//
// Parámetros:
// - str: La cadena de entrada a convertir.
//
// Retorno:
// - El valor entero obtenido de la cadena.
int string_to_int(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    return value;
}

// ========================================
//  FUNCIÓN: parseListenString
// ========================================
// Analiza una cadena de escucha (listen) y extrae la dirección IP y el puerto.
//
// Parámetros:
// - listen_str: Cadena en formato "IP:PUERTO" o solo "PUERTO".
//
// Retorno:
// - Un par de strings donde el primer elemento es la IP (vacía si no se especifica)
//   y el segundo elemento es el puerto.
std::pair<std::string, std::string> parseListenString(const std::string& listen_str) {
    std::pair<std::string, std::string> result;
    size_t pos = listen_str.find(':');
    if (pos != std::string::npos) {
        result.first = listen_str.substr(0, pos);   // Extrae la IP
        result.second = listen_str.substr(pos + 1); // Extrae el puerto
    } else {
        result.first = "";  // IP vacía
        result.second = listen_str;
    }
    return result;
}

// ========================================
//  FUNCIÓN: getResolvedAddress
// ========================================
// Resuelve la dirección IP y el puerto utilizando `getaddrinfo`.
//
// Parámetros:
// - ip: Dirección IP a resolver (vacía para INADDR_ANY).
// - port_str: Puerto en formato de cadena.
//
// Retorno:
// - Puntero a una estructura `addrinfo` con la información resuelta, o `NULL` si hay error.
struct addrinfo* getResolvedAddress(const std::string& ip, const std::string& port_str) {
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // Socket TCP
    hints.ai_flags = AI_PASSIVE;     // Usar INADDR_ANY si no hay IP

    struct addrinfo *res;
    int status = getaddrinfo(ip.empty() ? NULL : ip.c_str(), port_str.c_str(), &hints, &res);
    if (status != 0) {
        std::cerr << "Error en getaddrinfo con la IP: " 
                  << (ip.empty() ? "INADDR_ANY" : ip) << std::endl;
        return NULL;
    }
    return res;
}

// ========================================
//  FUNCIÓN: attemptBind
// ========================================
// Intenta vincular el socket a la dirección y puerto especificados.
//
// Parámetros:
// - server_fd: Descriptor del socket a vincular.
// - res: Puntero a `addrinfo` con la dirección resuelta.
// - ip: Dirección IP para la vinculación.
// - port: Número de puerto.
//
// Retorno:
// - 0 si la vinculación es exitosa, -1 si hay error.
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

// ========================================
//  FUNCIÓN: configure_and_bind_socket
// ========================================
// Configura y vincula un socket del servidor basándose en la configuración.
//
// Parámetros:
// - server_fd: Descriptor de archivo del socket del servidor.
// - config: Configuración del servidor.
// - i: Índice del servidor en la configuración.
//
// Retorno:
// - 0 si la configuración y vinculación fueron exitosas, -1 si hubo error.
int configure_and_bind_socket(int server_fd, std::pair<std::string, std::string> const &config) {
    // Obtener la cadena de escucha del servidor.
    std::string listen_str = config.first;
    
    // Extraer la IP y el puerto.
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