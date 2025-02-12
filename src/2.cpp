// 2️⃣ Configurar la dirección del servidor y Vincular el socket

#include "test.hpp"

int string_to_int(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    return value;
}

int paso_dos(int server_fd, Config *config) {
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr)); // Limpiamos la estructura

    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Escuchamos en todas las interfaces
    server_addr.sin_port = htons(string_to_int(config->v_servers[0]->v_listen[0])); // Convertir a int el puerto

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) { // Vinculamos el socket
        std::cerr << "Error al vincular el socket." << std::endl;
        return -1;
    }

    std::cout << "Se ha vinculado perfectamente el socket" << std::endl;

    if (paso_tres(server_fd, config) == -1) {
        return -1;
    }

    return 0;
}