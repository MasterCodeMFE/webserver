// 1️⃣ Crear el socket

#include "test.hpp"

void close_socket(int server_fd)
{
    close(server_fd);
}

int create_socket()
{
    int server_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0)
    {
        std::cerr << "Error al crear el socket." << std::endl;
        return (-1);
    }
    std::cout << "El socket se ha creado correctamente." << std::endl;
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cerr << "Error al configurar SO_REUSEADDR.\n";
        return -1;
    }
    return (server_fd);
}

int main()
{
    int server_fd;
    server_fd = create_socket();
    if(server_fd == -1)
    {
        return(1);
    }
    if(paso_dos(server_fd) == -1)
    {
        close_socket(server_fd);
        return(1);
    }
    
    close_socket(server_fd);
}