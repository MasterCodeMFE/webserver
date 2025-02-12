// 3️⃣ Poner el socket en modo de escucha

#include "test.hpp"

int paso_tres(int server_fd, Config *config)
{
    if (listen(server_fd, SOMAXCONN) == -1)
    {
        std::cerr << "Error al poner a la escucha el socket: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Servidor en escucha en el puerto " << config->v_servers[0]->v_listen[0] << "..." << std::endl;

    std::vector<pollfd> fds;

    struct pollfd server_pollfd = {};
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;
    fds.push_back(server_pollfd);

    while (true)
    {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1)
        {
            std::cerr << "Error en poll(): " << strerror(errno) << std::endl;
            return -1;
        }

        // Comprobar si hay una nueva conexión
        if (fds[0].revents & POLLIN)
        {
            int new_client_fd = paso_cuatro(server_fd);
            if (new_client_fd != -1)
            {
                struct pollfd new_pollfd = {};
                new_pollfd.fd = new_client_fd;
                new_pollfd.events = POLLIN;
                fds.push_back(new_pollfd);
            }
        }

        // Iterar sobre los clientes
        for (size_t i = 1; i < fds.size(); ++i)
        {
            if (fds[i].revents & (POLLIN | POLLHUP | POLLERR))
            {
                int res = paso_cinco(fds[i].fd);
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
