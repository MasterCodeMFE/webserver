/* 
    La función socket se utiliza para crear un nuevo descriptor de socket, que es un punto final de
    comunicación para enviar y recibir datos a través de una red. Permite a las aplicaciones establecer
    conexiones entre diferentes dispositivos en una red, ya sea en una red local o en Internet.

    Prototipo:
        int socket(int domain, int type, int protocol);

        Parámetros:
            domain: Especifica la familia de direcciones que se utilizará. Comúnmente se usa:
                AF_INET: Para IPv4.
                AF_INET6: Para IPv6.
            type: Especifica el tipo de socket. Los valores comunes son:
                SOCK_STREAM: Para un socket de flujo (TCP).
                SOCK_DGRAM: Para un socket de datagrama (UDP).
            protocol: Especifica el protocolo a utilizar. Generalmente se puede establecer en 0 para que
            el sistema seleccione el protocolo apropiado según los valores de domain y type.
        Valor de retorno:
            Devuelve el descriptor de socket (un número entero positivo) en caso de éxito.
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

    // Aquí podrías continuar con el uso del socket, como hacer un bind, listen, accept, etc.

    // Cerrar el socket al final
    close(sockfd);
    return 0;
}