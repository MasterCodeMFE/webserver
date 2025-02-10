/* 
    La función listen se utiliza en programación de sockets para indicar que un socket está dispuesto a
    aceptar conexiones entrantes. Debe ser llamada en un socket que ha sido creado y vinculado (con
    socket y bind). Esta función coloca el socket en modo de escucha y especifica el número máximo
    de conexiones en espera que el sistema permitirá.

    Prototipo:
        int listen(int sockfd, int backlog);

        Parámetros:
            sockfd: El descriptor de socket que se ha creado y vinculado previamente. Debe ser un socket de tipo SOCK_STREAM (TCP).
            backlog: Un entero que especifica el número máximo de conexiones que el sistema mantendrá
            en espera. Si el número de conexiones en espera supera este valor, las conexiones adicionales
            serán rechazadas hasta que se libere espacio.
        Valor de retorno:
            Devuelve 0 en caso de éxito.
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
    if (listen(sockfd, 5) < 0) {
        perror("Error en listen");
        close(sockfd);
        return 1;
    }

    printf("Servidor escuchando en el puerto 8080...\n");

    // Aquí podrías esperar a aceptar conexiones con accept()

    // Cerrar el socket al final
    close(sockfd);
    return 0;
}