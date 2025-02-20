// 2️⃣ Configurar la dirección del servidor y Vincular el socket

#include "test.hpp"

int string_to_int(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    return value;
}

int paso_dos(int server_fd, Config const &config, int i)
{
    struct addrinfo hints, *res;
    std::memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Usar la dirección IP del host si no hay una explícita

    // Obtener la IP y el puerto del servidor desde la configuración
    std::string listen_str = config.getVServers()[i]->getVListen()[0]; // Primer `listen`
    std::string ip = "";
    std::string port_str = listen_str;

    size_t pos = listen_str.find(':');
    if (pos != std::string::npos) {
        ip = listen_str.substr(0, pos);
        port_str = listen_str.substr(pos + 1);
    }

    int port = string_to_int(port_str);
    const char *ip_cstr;

    if (ip.empty()) {
        ip_cstr = NULL; // NULL para que escuche en todas las interfaces
    } else {
        ip_cstr = ip.c_str();
    }

    // Resolver dirección con getaddrinfo
    int status = getaddrinfo(ip_cstr, port_str.c_str(), &hints, &res);
    if (status != 0)
    {
        std::cerr << "Error en getaddrinfo con la IP: ";
        if (ip.empty()) {
            std::cerr << "INADDR_ANY";
        } else {
            std::cerr << ip;
        }
        std::cerr << std::endl;
        return -1;
    }

    std::cout << "Intentando vincular el socket en ";
    if (ip.empty()) {
        std::cout << "INADDR_ANY";
    } else {
        std::cout << ip;
    }
    std::cout << ":" << port << std::endl;

    if (bind(server_fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        std::cerr << "Error al vincular el socket en ";
        if (ip.empty()) {
            std::cerr << "INADDR_ANY";
        } else {
            std::cerr << ip;
        }
        std::cerr << ":" << port << ": " << strerror(errno) << std::endl;
        freeaddrinfo(res);
        return -1;
    }

    freeaddrinfo(res);
    std::cout << "Se ha vinculado correctamente en ";
    if (ip.empty()) {
        std::cout << "INADDR_ANY";
    } else {
        std::cout << ip;
    }
    std::cout << ":" << port << std::endl;
    
    return 0;
}