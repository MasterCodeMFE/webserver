// 2️⃣ Configurar la dirección del servidor y Vincular el socket

#include "test.hpp"

int paso_dos(int server_fd)
{
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr)); // Limpiamos la estructura
    server_addr.sin_family = AF_INET; //ipv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // escuchamos en todas la interfaces
    server_addr.sin_port = htons(PORT); //ponermos el puerto en el que va a escuchar nuestro servidor en formato de red
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) //binculamos el socket
    {
        std::cerr << "Error al vincular el socket." << std::endl;
        return(-1);
    }
    std::cout << "Se ha vimculado perfectamente el socket" << std::endl;
    if(paso_tres(server_fd) == -1)
    {
        return(-1);
    }

    return(0);
}