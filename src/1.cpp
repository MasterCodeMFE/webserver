// 1️⃣ Crear el socket

#include "test.hpp"

// Función para cerrar un socket.
void close_socket(int server_fd) {
    close(server_fd);
}

// Función para crear un socket y configurarlo (por ejemplo, con SO_REUSEADDR).
int create_and_configure_socket() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error al crear el socket." << std::endl;
        return -1;
    }
    std::cout << "El socket se ha creado correctamente." << std::endl;
    
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "Error al configurar SO_REUSEADDR." << std::endl;
        close_socket(server_fd);
        return -1;
    }
    return server_fd;
}

// Función que configura el socket de un servidor específico.
// Aquí se invoca la función 'paso_dos', que se asume configura el socket con la información del servidor.
int setup_server_socket(const Config &config, size_t index) {
    int server_fd = create_and_configure_socket();
    if (server_fd == -1) {
        std::cerr << "Error creando socket para servidor " << index << std::endl;
        return -1;
    }
    // Se asume que 'paso_dos' es responsable de realizar pasos adicionales (como el bind() o listen())
    if (paso_dos(server_fd, config, index) == -1) {
        close_socket(server_fd);
        return -1;
    }
    return server_fd;
}

// Función para inicializar todos los sockets de servidor a partir de la configuración.
// Se recorre cada servidor definido en la configuración y se intenta configurar su socket.
std::vector<int> init_server_sockets(const Config &config) {
    std::vector<int> server_fds;
    for (size_t i = 0; i < config.getVServers().size(); ++i) {
        int server_fd = setup_server_socket(config, i);
        if (server_fd != -1) {
            server_fds.push_back(server_fd);
        } else {
            std::cerr << "No se pudo configurar el socket para el servidor " << i << std::endl;
        }
    }
    return server_fds;
}

// Función principal que coordina la inicialización de los servidores y el manejo de conexiones.
// Se encarga de iniciar el polling (mediante 'paso_tres') y de cerrar los sockets una vez finalizado.
int paso_uno(const Config &config) {
    // Inicializa los sockets de todos los servidores.
    std::vector<int> server_fds = init_server_sockets(config);
    if (server_fds.empty()) {
        std::cerr << "No se pudieron inicializar los sockets del servidor." << std::endl;
        return 1;
    }

    // Se inicia el manejo de múltiples servidores (por ejemplo, con poll()).
    if (paso_tres(server_fds, config) == -1) {
        for (size_t i = 0; i < server_fds.size(); i++) {
            close_socket(server_fds[i]);
        }
        return 1;
    }

    // Cierre de todos los sockets una vez finalizado.
    for (size_t i = 0; i < server_fds.size(); i++) {
        close_socket(server_fds[i]);
    }
    return 0;
}
