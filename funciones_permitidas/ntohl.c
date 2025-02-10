/* 
    La función ntohl (Network TO Host Long) se utiliza en programación de red para convertir un
    número de 32 bits (un entero largo) desde el formato de byte de la red (big-endian) al formato de
    byte del host. Esto es fundamental porque diferentes arquitecturas de hardware pueden tener
    diferentes órdenes de bytes (big-endian o little-endian). ntohl asegura que el número se interprete
    correctamente según la arquitectura de la máquina local.

    Prototipo de la función:
        #include <netinet/in.h> // Necesario para ntohl

    uint32_t ntohl(uint32_t netlong);

    Parámetros:
        netlong: Un número de 32 bits en formato de byte de la red que se desea convertir al formato de byte del host.
    Valor de retorno:
        La función retorna el número convertido en formato de byte del host.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> // Para ntohl
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Usar SOCK_STREAM para TCP
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la estructura sockaddr_in para el servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Familia de direcciones (IPv4)
    server_addr.sin_addr.s_addr = INADDR_ANY; // Aceptar conexiones desde cualquier dirección
    server_addr.sin_port = htons(8080); // Convertir el puerto 8080 a formato de red

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

    // Aceptar una conexión entrante
    int client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_sock < 0) {
        perror("Error en accept");
        close(sockfd);
        return 1;
    }

    // Obtener la dirección IP del cliente
    uint32_t client_ip_net = client_addr.sin_addr.s_addr; // Dirección IP en formato de red
    uint32_t client_ip_host = ntohl(client_ip_net); // Convertir la dirección IP a formato de host

    printf("Conexión aceptada desde la dirección IP: %u.%u.%u.%u\n",
           (client_ip_host >> 24) & 0xFF, 
           (client_ip_host >> 16) & 0xFF, 
           (client_ip_host >> 8) & 0xFF, 
           client_ip_host & 0xFF);

    // Aquí puedes manejar la lógica del servidor...

    close(client_sock);
    close(sockfd);
    return 0;
}