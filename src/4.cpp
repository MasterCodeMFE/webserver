// 4️⃣ Aceptar una conexión entrante

#include "test.hpp"

// Función para aceptar la conexión del cliente
int acceptConnection(int server_fd) {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
    if (client_fd == -1) {
        std::cerr << "Error al aceptar la conexión: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Nuevo cliente conectado: " << client_fd << "\n";
    }
    return client_fd;
}

// Función para configurar el socket en modo no bloqueante
void setNonBlocking(int client_fd) {
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "Error al configurar el socket como no bloqueante: " << strerror(errno) << std::endl;
    }
}

// Función para habilitar la opción keep-alive
void enableKeepAlive(int client_fd) {
    int optval = 1;
    if (setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0) {
        std::cerr << "Error al habilitar keep-alive: " << strerror(errno) << std::endl;
    }
}

// Función para configurar los tiempos de keep-alive
void configureKeepAliveTimes(int client_fd, int keep_idle = 5, int keep_intvl = 5, int keep_cnt = 3) {
    if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPIDLE, &keep_idle, sizeof(keep_idle)) < 0) {
        std::cerr << "Error al configurar TCP_KEEPIDLE: " << strerror(errno) << std::endl;
    }
    if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPINTVL, &keep_intvl, sizeof(keep_intvl)) < 0) {
        std::cerr << "Error al configurar TCP_KEEPINTVL: " << strerror(errno) << std::endl;
    }
    if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPCNT, &keep_cnt, sizeof(keep_cnt)) < 0) {
        std::cerr << "Error al configurar TCP_KEEPCNT: " << strerror(errno) << std::endl;
    }
}

// Función principal que utiliza las anteriores para manejar la conexión del cliente
int paso_cuatro(int server_fd, Config const &config) {
    (void)config; // Se ignora config en este ejemplo, pero puede utilizarse si es necesario
    int client_fd = acceptConnection(server_fd);
    if (client_fd == -1) {
        return -1;
    }

    setNonBlocking(client_fd);
    enableKeepAlive(client_fd);
    configureKeepAliveTimes(client_fd);

    return client_fd;
}
