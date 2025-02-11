/* 
    La función send se utiliza para enviar datos a través de un socket conectado. Es comúnmente
    utilizada en aplicaciones de red para enviar datos desde un cliente a un servidor o viceversa. Esta
    función toma como argumentos el descriptor del socket, un puntero a los datos que se desean
    enviar, el tamaño de esos datos y un conjunto de opciones que pueden modificar el comportamiento
    de la operación.

    Prototipo:
        ssize_t send(int sockfd, const void *buf, size_t len, int flags);

        Parámetros:
            sockfd: El descriptor del socket a través del cual se enviarán los datos. Debe ser un socket que esté conectado.
            buf: Un puntero a los datos que se desean enviar. Puede ser un puntero a un buffer que contiene una cadena, una estructura, etc.
            len: El número de bytes que se desean enviar desde el buffer. Este valor debe ser menor o igual al tamaño del buffer.
            flags: Opciones que modifican el comportamiento de la función. Se pueden usar valores como
            0 para el comportamiento por defecto, MSG_OOB para enviar datos fuera de banda, etc.
        Valor de retorno:
            Devuelve el número de bytes enviados en caso de éxito.
            Devuelve -1 en caso de error y establece errno para indicar el tipo de error.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd;
    struct sockaddr_in server_address;
    const char *message = "Hola, servidor!";
    
    // Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al abrir el socket");
        return 1;
    }

    // Configurar la dirección del servidor
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET; // IPv4
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección del servidor
    server_address.sin_port = htons(8080); // Puerto 8080

    // Conectar al servidor
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error en la conexión");
        close(sockfd);
        return 1;
    }

    // Enviar un mensaje al servidor
    ssize_t bytes_sent = send(sockfd, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("Error en send");
        close(sockfd);
        return 1;
    }

    printf("Se enviaron %zd bytes: %s\n", bytes_sent, message);

    // Cerrar el socket al final
    close(sockfd);
    return 0;
}