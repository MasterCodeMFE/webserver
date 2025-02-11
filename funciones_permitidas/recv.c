/* 
    La función recv se utiliza para recibir datos de un socket conectado. Es comúnmente utilizada en
    aplicaciones de red para recibir datos que han sido enviados por otro socket. Esta función permite
    especificar el tamaño del buffer que se usará para almacenar los datos recibidos y opciones que
    pueden modificar su comportamiento.

    Prototipo:
        ssize_t recv(int sockfd, void *buf, size_t len, int flags);

        Parámetros:
            sockfd: El descriptor del socket a través del cual se recibirán los datos. Debe ser un socket que esté conectado.
            buf: Un puntero a un buffer donde se almacenarán los datos recibidos. Este buffer debe ser
            suficientemente grande para contener los datos que se esperan recibir.
            len: El número máximo de bytes que se desean recibir y almacenar en el buffer. Este valor debe ser mayor que cero.
            flags: Opciones que modifican el comportamiento de la función. Se pueden usar valores como
            0 para el comportamiento por defecto, MSG_OOB para recibir datos fuera de banda, entre otros.
        Valor de retorno:
            Devuelve el número de bytes recibidos en caso de éxito.
            Devuelve 0 si se ha cerrado la conexión de forma ordenada.
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
    int sockfd, newsockfd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_len;
    char buffer[256];

    // Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al abrir el socket");
        return 1;
    }

    // Configurar la dirección del servidor
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET; // IPv4
    server_address.sin_addr.s_addr = INADDR_ANY; // Aceptar conexiones de cualquier dirección
    server_address.sin_port = htons(8080); // Puerto 8080

    // Hacer bind del socket
    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error en el bind");
        close(sockfd);
        return 1;
    }

    // Poner el socket en modo de escucha
    listen(sockfd, 5);

    // Aceptar una conexión entrante
    client_len = sizeof(client_address);
    newsockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_len);
    if (newsockfd < 0) {
        perror("Error en accept");
        close(sockfd);
        return 1;
    }

    // Recibir un mensaje del cliente
    ssize_t bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("Error en recv");
        close(newsockfd);
        close(sockfd);
        return 1;
    }

    // Terminar la cadena recibida y mostrarla
    buffer[bytes_received] = '\0';
    printf("Mensaje recibido: %s\n", buffer);

    // Cerrar los sockets
    close(newsockfd);
    close(sockfd);
    return 0;
}