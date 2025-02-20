// 3️⃣ Poner el socket en modo de escucha

#include "test.hpp"

int paso_tres(std::vector<int> server_fds, Config const &config)
{
    std::vector<pollfd> fds;

    // Configurar `listen()` para cada servidor y agregarlo al vector de `pollfd`
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

        struct pollfd server_pollfd = {};
        server_pollfd.fd = server_fds[i];
        server_pollfd.events = POLLIN | POLLOUT;
        fds.push_back(server_pollfd);
    }

    while (true)
    {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1)
        {
            std::cerr << "Error en poll(): " << strerror(errno) << std::endl;
            return -1;
        }

        // Revisar cada socket de servidor
        for (size_t i = 0; i < server_fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                int new_client_fd = paso_cuatro(server_fds[i], config);
                if (new_client_fd != -1)
                {
                    struct pollfd new_pollfd = {};
                    new_pollfd.fd = new_client_fd;
                    new_pollfd.events = POLLIN | POLLOUT;
                    fds.push_back(new_pollfd);
                }
            }
        }

        // Iterar sobre los clientes conectados
        for (size_t i = server_fds.size(); i < fds.size(); ++i)
        {
            if (fds[i].revents & (POLLIN | POLLHUP | POLLERR))
            {
                int res = paso_cinco(fds[i].fd, config);
                if (res == -1) // Cliente desconectado
                {
                    std::cout << "Cliente desconectado: " << fds[i].fd << "\n";
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    --i;
                }
            }
        }
    }
    return 0;
}
