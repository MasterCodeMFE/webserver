/* 
    La función connect se utiliza para establecer una conexión a un socket de red. Es comúnmente
    utilizada en clientes de aplicaciones de red para conectarse a un servidor en una dirección IP y puerto
    específicos. La función intenta establecer una conexión TCP con el servidor especificado en la
    estructura de dirección.

    Prototipo:
        int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

        Parámetros:
            sockfd: El descriptor del socket que se va a usar para la conexión.
            addr: Un puntero a una estructura sockaddr que contiene la dirección (IP y puerto) del servidor al que se desea conectar.
            addrlen: El tamaño, en bytes, de la estructura sockaddr a la que apunta addr.
        Valor de retorno:
            Devuelve 0 si la conexión se establece con éxito.
            Devuelve -1 si hay un error y establece errno para indicar el tipo de error (como no se pudo conectar, dirección no disponible, etc.).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // Dirección IP del servidor
#define PORT 8080 // Puerto al que el cliente se conectará

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
    server_addr.sin_port = htons(PORT); // Asignar el puerto
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // Asignar la dirección IP del servidor

    // Establecer la conexión con el servidor
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en connect");
        close(sockfd);
        return 1;
    }

    printf("Conectado al servidor %s en el puerto %d\n", SERVER_IP, PORT);

    // Aquí iría el código para enviar/recibir datos

    // Cerrar el socket
    close(sockfd);
    return 0;
}