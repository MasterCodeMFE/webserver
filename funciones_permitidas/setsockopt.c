/* 
    La función setsockopt se utiliza para configurar opciones de socket. Permite modificar el
    comportamiento de un socket especificando diferentes opciones de nivel de socket o de protocolo.
    Esta función es fundamental para ajustar configuraciones como el tamaño de los buffers, el tiempo
    de espera para la conexión, la reutilización de direcciones, entre otras.

    Prototipo:
        int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

        Parámetros:
            sockfd: El descriptor del socket que se va a modificar.
            level: El nivel al que se aplica la opción. Por lo general, se utiliza SOL_SOCKET para opciones de socket o un nivel de protocolo específico.
            optname: El nombre de la opción que se va a establecer (por ejemplo, SO_REUSEADDR).
            optval: Un puntero a la opción que se va a establecer. El tipo de este argumento depende de la opción seleccionada.
            optlen: El tamaño de la opción especificada (en bytes).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    int optval = 1; // Valor para permitir la reutilización de la dirección
    socklen_t optlen = sizeof(optval);

    // Crear un socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Establecer la opción SO_REUSEADDR
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) < 0) {
        perror("Error al establecer la opción SO_REUSEADDR");
        close(sockfd);
        return 1;
    }

    // Configurar la dirección y puerto del socket
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Aceptar conexiones en cualquier dirección
    server_addr.sin_port = htons(8080); // Puerto 8080

    // Asociar el socket a la dirección y puerto
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al asociar el socket");
        close(sockfd);
        return 1;
    }

    // Escuchar conexiones entrantes
    if (listen(sockfd, 5) < 0) {
        perror("Error al escuchar conexiones");
        close(sockfd);
        return 1;
    }

    printf("Servidor escuchando en el puerto 8080...\n");

    // Aquí se puede agregar el código para aceptar conexiones

    // Cerrar el socket
    close(sockfd);
    return 0;
}