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

Config *initializeConfig()
{
    Config *config = new Config();

    // Crear un servidor por defecto
    Server *server = new Server();

    // Inicialización manual de los valores del servidor
    server->server_name = "localhost";
    server->v_listen.push_back("8080");
    server->client_max_body = 1048576; // 1MB
    server->autoindex = false;
    server->index = "index.html";
    server->cgi = "/usr/bin/php";
    server->root = "/var/www/html";

    // Configurar páginas de error por defecto
    server->m_status_pages.push_back(new Status(404, "/errors/404.html"));
    server->m_status_pages.push_back(new Status(500, "/errors/500.html"));

    // Crear una ubicación por defecto
    Location *location = new Location();
    location->root = "/var/www/html";
    location->index = "index.html";
    location->autoindex = false;
    location->v_methods.push_back("GET");
    location->v_methods.push_back("POST");
    location->v_methods.push_back("DELETE");

    server->m_locations.push_back(location);

    // Añadir el servidor a la configuración
    config->v_servers.push_back(server);

    return config;
}


int main()
{

    int server_fd;
    Config *config = initializeConfig();
    server_fd = create_socket();
    if(server_fd == -1)
    {
        return(1);
    }
    if(paso_dos(server_fd, config) == -1)
    {
        close_socket(server_fd);
        return(1);
    }
    delete config;
    close_socket(server_fd);
}