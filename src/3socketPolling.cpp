// 3️⃣ Poner el socket en modo de escucha

#include "test.hpp"

// ========================================
//  FUNCIÓN: setup_server_listeners
// ========================================
// Configura el modo de escucha para cada socket de servidor y los agrega
// al vector de `pollfd` para la monitorización de eventos.
//
// Parámetros:
// - server_fds: Vector de descriptores de socket de los servidores.
// - config: Configuración que contiene información de cada servidor.
// - fds: Vector donde se agregarán los `pollfd` configurados.
//
// Retorno:
// - 0 si todo fue bien.
// - -1 en caso de error.
int setup_server_listeners(const std::vector<int>& server_fds,
                           const Config& config,
                           std::vector<pollfd>& fds)
{
    for (size_t i = 0; i < server_fds.size(); i++)
    {
        if (listen(server_fds[i], SOMAXCONN) == -1)
        {
            std::cerr << "[ERROR] No se pudo poner en escucha el socket en "
                      << config.getVServers()[i]->getVListen()[0] << " ("
                      << config.getVServers()[i]->getServerName()[0] << "): "
                      << strerror(errno) << std::endl;
            return -1;
        }

        std::cout << "[INFO] Servidor en escucha en "
                  << config.getVServers()[i]->getServerName()[0]
                  << ":" << config.getVServers()[i]->getVListen()[0]
                  << std::endl;

        // Configurar el socket en poll para monitorizar eventos de entrada y salida.
        pollfd server_pollfd = {};
        server_pollfd.fd = server_fds[i];
        server_pollfd.events = POLLIN | POLLOUT;
        fds.push_back(server_pollfd);
    }
    return 0;
}

// ========================================
//  FUNCIÓN: process_server_events
// ========================================
// Procesa los eventos de los sockets de servidor.
//
// Si se detecta un evento de lectura (`POLLIN`), se acepta la conexión
// y se agrega el nuevo socket de cliente al vector `fds`.
//
// Parámetros:
// - server_fds: Vector de descriptores de socket de los servidores.
// - config: Configuración general.
// - fds: Vector de `pollfd` donde se registran los sockets (tanto de servidores como de clientes).
void process_server_events(const std::vector<int>& server_fds,
                           const Config& config,
                           std::vector<pollfd>& fds)
{
    for (size_t i = 0; i < server_fds.size(); i++)
    {
        if (fds[i].revents & POLLIN)
        {
            int new_client_fd = accept_client_connection(server_fds[i], config);
            if (new_client_fd != -1)
            {
                // Se configura el nuevo socket de cliente en `poll`
                pollfd new_pollfd = {};
                new_pollfd.fd = new_client_fd;
                new_pollfd.events = POLLIN | POLLOUT;
                fds.push_back(new_pollfd);
            }
        }
    }
}

// ========================================
//  FUNCIÓN: process_client_events
// ========================================
// Procesa los eventos de los sockets correspondientes a clientes.
//
// Si un cliente se desconecta (`handle_client_request` retorna -1),
// se cierra el socket y se elimina del vector.
//
// Parámetros:
// - start_index: Índice desde el cual inician los sockets de cliente en el vector `fds`.
// - config: Configuración general.
// - fds: Vector de `pollfd` que contiene tanto servidores como clientes.
void process_client_events(size_t start_index,
                           const Config& config,
                           std::vector<pollfd>& fds)
{
    for (size_t i = start_index; i < fds.size(); ++i)
    {
        if (fds[i].revents & (POLLIN | POLLHUP | POLLERR))
        {
            int res = handle_client_request(fds[i].fd, config);
            if (res == -1)  // Cliente desconectado
            {
                std::cout << "[INFO] Cliente desconectado: " << fds[i].fd << "\n";
                close(fds[i].fd);
                fds.erase(fds.begin() + i);
                --i; // Ajustar índice tras la eliminación
            }
        }
    }
}

// ========================================
//  FUNCIÓN: run_server_event_loop
// ========================================
// Función principal que maneja las conexiones del servidor.
//
// Se encarga de:
// 1. Configurar los sockets de escucha.
// 2. Entrar en el bucle principal de eventos (`poll`).
// 3. Procesar los eventos de los servidores y clientes.
//
// Parámetros:
// - server_fds: Vector de descriptores de socket de los servidores.
// - config: Configuración general.
//
// Retorno:
// - 0 si todo se ejecuta correctamente.
// - -1 en caso de error.
int run_server_event_loop(const std::vector<int>& server_fds, const Config& config)
{
    std::vector<pollfd> fds;

    // Configurar los sockets en modo de escucha y añadirlos a `pollfd`
    if (setup_server_listeners(server_fds, config, fds) == -1)
        return -1;

    while (true)
    {
        // Esperar eventos en los sockets registrados
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1)
        {
            std::cerr << "[ERROR] Error en poll(): " << strerror(errno) << std::endl;
            return -1;
        }

        // Procesar eventos de los servidores
        process_server_events(server_fds, config, fds);

        // Procesar eventos de los clientes conectados
        process_client_events(server_fds.size(), config, fds);
    }
    return 0;
}
