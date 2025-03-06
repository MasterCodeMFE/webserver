/* 
    La función htons (Host TO Network Short) se utiliza en programación de red para convertir un
    número de 16 bits (un short) desde el formato de byte del host al formato de byte de la red (big-endian).
    Esto es importante porque diferentes arquitecturas de hardware pueden usar diferentes
    órdenes de bytes para almacenar datos. htons se asegura de que el número se almacene en el
    formato correcto para su transmisión a través de redes.

    Prototipo de la función:
        #include <netinet/in.h> // Necesario para htons

        uint16_t htons(uint16_t hostshort);

        Parámetros:
            hostshort: Un número de 16 bits en formato de byte del host que se desea convertir a formato de byte de red.
        Valor de retorno:
            La función retorna el número convertido en formato de byte de red.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> // Para htons
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la estructura sockaddr_in
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;                // Familia de direcciones (IPv4)
    server_addr.sin_addr.s_addr = INADDR_ANY;      // Aceptar conexiones desde cualquier dirección
    server_addr.sin_port = htons(8080);              // Convertir el puerto 8080 a formato de red

    // Enlazar el socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        close(sockfd);
        return 1;
    }

    // Escuchar conexiones entrantes
    if (listen(sockfd, 5) < 0) {
        perror("Error en listen");
        close(sockfd);
        return 1;
    }

    printf("Servidor escuchando en el puerto 8080...\n");

    // Aquí puedes aceptar conexiones y manejar la lógica del servidor...

    close(sockfd);
    return 0;
}