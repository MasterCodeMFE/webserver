/* 
    La función bind se utiliza para asignar una dirección específica (IP y puerto) a un socket. Esto es
    esencial para que un socket pueda recibir conexiones en un puerto específico. Generalmente, se
    utiliza en aplicaciones de red, como servidores, que necesitan asociar un socket a un puerto en el que
    escucharán las conexiones entrantes.

    Prototipo:
        int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

        Parámetros:
            sockfd: El descriptor del socket que se desea asociar a la dirección.
            addr: Un puntero a una estructura sockaddr que contiene la dirección (IP y puerto) que se desea asociar al socket.
            addrlen: El tamaño, en bytes, de la estructura sockaddr a la que apunta addr.
        Valor de retorno:
            Devuelve 0 si la operación tiene éxito.
            Devuelve -1 si hay un error y establece errno para indicar el tipo de error (como dirección ya en uso o permisos insuficientes).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080 // Puerto en el que el servidor escuchará

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la dirección del servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Familia de direcciones IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Aceptar conexiones en cualquier dirección
    server_addr.sin_port = htons(PORT); // Asignar el puerto, asegurando el orden de bytes correcto

    // Asociar el socket a la dirección
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        close(sockfd);
        return 1;
    }

    // Escuchar conexiones (opcional)
    if (listen(sockfd, 5) < 0) {
        perror("Error en listen");
        close(sockfd);
        return 1;
    }

    printf("Servidor escuchando en el puerto %d\n", PORT);
    
    // Aquí iría el código para aceptar conexiones

    // Cerrar el socket
    close(sockfd);
    return 0;
}