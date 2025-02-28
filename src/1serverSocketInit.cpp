// 1️⃣ Crear el socket

#include "test.hpp"

// ========================================
//  FUNCIÓN: close_socket
// ========================================
// Cierra el socket especificado y libera los recursos asociados.
//
// Parámetros:
// - server_fd: Descriptor del socket a cerrar.
//
// Esta función es importante para evitar fugas de recursos en el sistema operativo.
void close_socket(int server_fd) {
    close(server_fd); // Llamada al sistema para cerrar el socket.
}

// ========================================
//  FUNCIÓN: create_and_configure_socket
// ========================================
// Crea un socket TCP y le asigna la opción SO_REUSEADDR.
//
// Retorno:
// - Descriptor de archivo del socket si la creación y configuración tienen éxito.
// - -1 si ocurre un error.
//
// SO_REUSEADDR permite reutilizar una dirección y puerto sin esperar el timeout del SO.
// Esto es útil en servidores que pueden reiniciarse rápidamente.
int create_and_configure_socket() {
    // Se crea un socket de dominio IPv4 (AF_INET) y tipo TCP (SOCK_STREAM).
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { // Verificar si hubo error en la creación del socket.
        std::cerr << "Error al crear el socket." << std::endl;
        return -1;
    }
    std::cout << "El socket se ha creado correctamente." << std::endl;
    
    // Configurar la opción SO_REUSEADDR.
    // Esto evita problemas al reiniciar el servidor, permitiendo reutilizar la dirección de inmediato.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "Error al configurar SO_REUSEADDR." << std::endl;
        close_socket(server_fd); // Cerrar el socket si la configuración falla.
        return -1;
    }
    return server_fd; // Retornar el descriptor del socket configurado.
}

// ========================================
//  FUNCIÓN: setup_server_socket
// ========================================
// Configura un socket para un servidor específico.
//
// Parámetros:
// - config: Objeto que contiene la configuración del servidor.
// - index: Índice del servidor en la configuración.
//
// Retorno:
// - Descriptor del socket configurado si tiene éxito.
// - -1 si ocurre un error.
//
// Esta función realiza dos pasos:
// 1. Crea y configura un socket con `create_and_configure_socket()`.
// 2. Llama a `configure_and_bind_socket()` para enlazarlo y prepararlo.
int setup_server_socket(const Config &config, size_t index) {
    // Se crea el socket para el servidor en la posición `index`.
    int server_fd = create_and_configure_socket();
    if (server_fd == -1) { // Si no se pudo crear el socket, se devuelve error.
        std::cerr << "Error creando socket para servidor " << index << std::endl;
        return -1;
    }

    // `configure_and_bind_socket` debe encargarse de hacer bind() y posiblemente listen().
    if (configure_and_bind_socket(server_fd, config, index) == -1) {
        close_socket(server_fd); // Si la configuración falla, se cierra el socket.
        return -1;
    }
    return server_fd; // Retorna el descriptor del socket correctamente configurado.
}

// ========================================
//  FUNCIÓN: init_server_sockets
// ========================================
// Inicializa los sockets de todos los servidores definidos en la configuración.
//
// Parámetros:
// - config: Objeto con la configuración de los servidores.
//
// Retorno:
// - Vector con los descriptores de los sockets configurados.
// - Si no se pudo inicializar ningún socket, devuelve un vector vacío.
//
// Esta función recorre la lista de servidores en `config.getVServers()` y configura un socket para cada uno.
std::vector<int> init_server_sockets(const Config &config) {
    std::vector<int> server_fds; // Vector donde se almacenarán los descriptores de socket creados.

    // Se recorren todos los servidores definidos en la configuración.
    for (size_t i = 0; i < config.getVServers().size(); ++i) {
        int server_fd = setup_server_socket(config, i);
        if (server_fd != -1) { // Si el socket se configuró correctamente, se agrega al vector.
            server_fds.push_back(server_fd);
        } else {
            std::cerr << "No se pudo configurar el socket para el servidor " << i << std::endl;
        }
    }
    return server_fds; // Se devuelve el vector con los sockets configurados.
}

// ========================================
//  FUNCIÓN: initialize_server_sockets
// ========================================
// Función principal que coordina la inicialización y gestión de los servidores.
//
// Parámetros:
// - config: Configuración del servidor.
//
// Retorno:
// - 0 si la inicialización y ejecución fueron exitosas.
// - 1 si hubo algún error.
//
// Este flujo sigue tres pasos:
// 1. Se inicializan los sockets de los servidores.
// 2. Se llama a `run_server_event_loop()` para manejar las conexiones de los clientes.
// 3. Se cierran todos los sockets al finalizar.
int initialize_server_sockets(const Config &config) {
    // Inicializa los sockets de todos los servidores según la configuración.
    std::vector<int> server_fds = init_server_sockets(config);
    if (server_fds.empty()) { // Si no se pudo configurar ningún socket, se reporta un error.
        std::cerr << "No se pudieron inicializar los sockets del servidor." << std::endl;
        return 1; // Se retorna error.
    }

    // Se inicia el bucle de eventos para manejar múltiples conexiones de clientes.
    // Se asume que `run_server_event_loop` utiliza `poll()`, `select()`, o un mecanismo similar.
    if (run_server_event_loop(server_fds, config) == -1) {
        // Si hay un error durante la ejecución del servidor, se cierran los sockets abiertos.
        for (size_t i = 0; i < server_fds.size(); i++) {
            close_socket(server_fds[i]);
        }
        return 1;
    }

    // Si el servidor finaliza correctamente, se cierran todos los sockets antes de salir.
    for (size_t i = 0; i < server_fds.size(); i++) {
        close_socket(server_fds[i]);
    }
    return 0; // Retorna éxito.
}