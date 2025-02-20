// 3️⃣ Poner el socket en modo de escucha

#include "test.hpp"

/**
 * Configura el modo de escucha para cada socket de servidor y los agrega al vector de pollfd.
 *
 * @param server_fds Vector de descriptores de socket de los servidores.
 * @param config Configuración que contiene información de cada servidor.
 * @param fds Vector donde se agregarán los pollfd configurados.
 * @return 0 si todo fue bien, -1 en caso de error.
 */
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

        pollfd server_pollfd = {};
        server_pollfd.fd = server_fds[i];
        server_pollfd.events = POLLIN | POLLOUT;
        fds.push_back(server_pollfd);
    }
    return 0;
}

/**
 * Procesa los eventos de los sockets de servidor.
 *
 * Si se detecta un evento de lectura (POLLIN) en alguno de ellos, se llama a `paso_cuatro`
 * para aceptar la conexión y, de ser exitosa, se agrega el nuevo socket de cliente al vector de pollfd.
 *
 * @param server_fds Vector de descriptores de socket de los servidores.
 * @param config Configuración general.
 * @param fds Vector de pollfd donde se registran los sockets (tanto de servidores como de clientes).
 */
void process_server_events(const std::vector<int>& server_fds,
                           const Config& config,
                           std::vector<pollfd>& fds)
{
    for (size_t i = 0; i < server_fds.size(); i++)
    {
        if (fds[i].revents & POLLIN)
        {
            int new_client_fd = paso_cuatro(server_fds[i], config);
            if (new_client_fd != -1)
            {
                pollfd new_pollfd = {};
                new_pollfd.fd = new_client_fd;
                new_pollfd.events = POLLIN | POLLOUT;
                fds.push_back(new_pollfd);
            }
        }
    }
}

/**
 * Procesa los eventos de los sockets correspondientes a clientes.
 *
 * Se itera a partir del índice `start_index` (que normalmente es igual al tamaño del vector de servidores)
 * y se verifica si existen eventos en el socket del cliente (como POLLIN, POLLHUP o POLLERR).
 * Si el cliente se desconecta (paso_cinco retorna -1), se cierra el socket y se elimina del vector.
 *
 * @param start_index Índice desde el cual se inician los sockets de cliente en el vector `fds`.
 * @param config Configuración general.
 * @param fds Vector de pollfd que contiene tanto servidores como clientes.
 */
void process_client_events(size_t start_index,
                           const Config& config,
                           std::vector<pollfd>& fds)
{
    for (size_t i = start_index; i < fds.size(); ++i)
    {
        if (fds[i].revents & (POLLIN | POLLHUP | POLLERR))
        {
            int res = paso_cinco(fds[i].fd, config);
            if (res == -1)  // Cliente desconectado
            {
                std::cout << "Cliente desconectado: " << fds[i].fd << "\n";
                close(fds[i].fd);
                fds.erase(fds.begin() + i);
                --i;
            }
        }
    }
}

/**
 * Función principal que orquesta el manejo de conexiones.
 *
 * Se encarga de inicializar los sockets de escucha, entrar en el loop principal
 * y delegar el procesamiento de eventos en funciones específicas para servidores y clientes.
 *
 * @param server_fds Vector de descriptores de socket de los servidores.
 * @param config Configuración general.
 * @return 0 si todo se ejecuta correctamente, -1 en caso de error.
 */
int paso_tres(const std::vector<int>& server_fds, const Config& config)
{
    std::vector<pollfd> fds;

    if (setup_server_listeners(server_fds, config, fds) == -1)
        return -1;

    while (true)
    {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1)
        {
            std::cerr << "Error en poll(): " << strerror(errno) << std::endl;
            return -1;
        }

        // Procesar eventos de los sockets de servidor
        process_server_events(server_fds, config, fds);

        // Procesar eventos de los clientes conectados
        process_client_events(server_fds.size(), config, fds);
    }
    return 0;
}
