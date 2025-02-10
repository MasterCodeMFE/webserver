/* 
    La función accept se utiliza en programación de sockets para aceptar una conexión entrante en un
    socket que está en modo de escucha (listening). Esta función bloquea el proceso hasta que un cliente
    intenta conectarse, y una vez que se acepta la conexión, devuelve un nuevo descriptor de socket que
    se puede utilizar para la comunicación con el cliente.

    Prototipo:
        int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

        Parámetros:
            sockfd: El descriptor de socket que se ha creado previamente y está en modo de escucha
            (resultado de socket() seguido de bind() y listen()).
            addr: Un puntero a una estructura sockaddr donde se almacenará la dirección del cliente que
            se está conectando. Puede ser NULL si no se necesita esta información.
            addrlen: Un puntero a una variable que contiene el tamaño de la estructura addr. Al salir de la
            función, esta variable se actualizará con el tamaño de la dirección del cliente.
        Valor de retorno:
            Devuelve un nuevo descriptor de socket (un número entero positivo) en caso de éxito, que se utiliza para comunicarse con el cliente.
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
    socklen_t client_len = sizeof(client_address);

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

    // Escuchar conexiones entrantes
    listen(sockfd, 5);

    // Aceptar una conexión
    newsockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_len);
    if (newsockfd < 0) {
        perror("Error al aceptar la conexión");
        close(sockfd);
        return 1;
    }

    printf("Conexión aceptada desde %s:%d\n", 
           inet_ntoa(client_address.sin_addr), 
           ntohs(client_address.sin_port));

    // Aquí puedes continuar con la comunicación usando newsockfd

    // Cerrar los sockets al final
    close(newsockfd);
    close(sockfd);
    return 0;
}