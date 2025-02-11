/* 
    La función getsockname se utiliza para obtener la dirección local y el puerto de un socket. Esto es
    útil cuando se desea conocer la dirección a la que está vinculado un socket, especialmente después
    de haberlo asociado a una dirección y puerto utilizando la función bind. La información se devuelve
    en una estructura que contiene detalles sobre la dirección del socket.

    Prototipo:
        int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

        Parámetros:
            sockfd: El descriptor del socket cuyo nombre se desea obtener.
            addr: Un puntero a una estructura sockaddr donde se almacenará la dirección del socket.
            addrlen: Un puntero a una variable que contiene el tamaño de la estructura addr en bytes. La
            función getsockname actualizará esta variable con el tamaño real de la dirección almacenada.
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
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    // Crear un socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la dirección y puerto del socket
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

    // Obtener la dirección del socket
    if (getsockname(sockfd, (struct sockaddr *)&server_addr, &addr_len) < 0) {
        perror("Error al obtener el nombre del socket");
        close(sockfd);
        return 1;
    }

    // Mostrar la dirección y el puerto
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_addr.sin_addr, ip, sizeof(ip));
    printf("Dirección IP: %s\n", ip);
    printf("Puerto: %d\n", ntohs(server_addr.sin_port));

    // Cerrar el socket
    close(sockfd);
    return 0;
}