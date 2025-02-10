// 4️⃣ Aceptar una conexión entrante

#include "test.hpp"

int paso_cuatro(int server_fd)
{
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    
    if (client_fd == -1)
    {
        std::cerr << "Error al aceptar la conexión: " << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "Nuevo cliente conectado: " << client_fd << "\n";

    // 🔹 Configurar socket para que sea no bloqueante
    fcntl(client_fd, F_SETFL, O_NONBLOCK);

    // 🔹 Habilitar keep-alive en la conexión
    int optval = 1;
    setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));

    // 🔹 Configurar tiempos de keep-alive
    int keep_idle = 5;   
    int keep_intvl = 5;  
    int keep_cnt = 3;    

    setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPIDLE, &keep_idle, sizeof(keep_idle));
    setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPINTVL, &keep_intvl, sizeof(keep_intvl));
    setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPCNT, &keep_cnt, sizeof(keep_cnt));

    return client_fd;
}
