/* 
    La función select se utiliza en programación de sockets para monitorear múltiples descriptores de
    archivo (como sockets) para ver si alguno de ellos está listo para realizar operaciones de lectura,
    escritura o si hay excepciones. Esto permite a una aplicación manejar múltiples conexiones sin
    necesidad de crear un hilo o proceso separado para cada conexión. select puede ser
    particularmente útil en servidores que manejan múltiples clientes de manera simultánea.

    Prototipo de la función:
        #include <sys/select.h> // Necesario para select
        #include <sys/time.h>

        int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
        Parámetros:
            nfds: El número máximo de descriptores de archivo que se van a supervisar. Este valor debe
            ser mayor que el mayor descriptor de archivo en cualquiera de los conjuntos.
            readfds: Un puntero a un conjunto de descriptores de archivo que se comprobarán para ver si están listos para leer.
            writefds: Un puntero a un conjunto de descriptores de archivo que se comprobarán para ver si están listos para escribir.
            exceptfds: Un puntero a un conjunto de descriptores de archivo que se comprobarán para excepciones.
            timeout: Un puntero a una estructura timeval que especifica el tiempo de espera máximo
            para que select bloquee. Si se establece en NULL, select bloqueará indefinidamente.
        Valor de retorno:
            Retorna el número de descriptores de archivo listos, o 0 si se alcanza el tiempo de espera, o -1 si ocurre un error.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    fd_set readfds;
    struct timeval timeout;

    // Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la dirección del servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conectar al servidor
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al conectar");
        close(sockfd);
        return 1;
    }

    // Configurar el conjunto de descriptores de archivo para select
    FD_ZERO(&readfds); // Inicializa el conjunto
    FD_SET(sockfd, &readfds); // Agrega el socket
    FD_SET(STDIN_FILENO, &readfds); // Agrega stdin

    // Establecer el tiempo de espera
    timeout.tv_sec = 5; // Espera 5 segundos
    timeout.tv_usec = 0;

    printf("Esperando por datos...\n");

    // Monitorear los descriptores de archivo
    int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
    if (activity < 0) {
        perror("Error en select");
        close(sockfd);
        return 1;
    } else if (activity == 0) {
        printf("Tiempo de espera alcanzado. No hay actividad.\n");
    } else {
        // Comprobar si hay datos disponibles en el socket
        if (FD_ISSET(sockfd, &readfds)) {
            char buffer[1024];
            ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0'; // Asegurarse de que sea una cadena
                printf("Datos recibidos del servidor: %s\n", buffer);
            }
        }

        // Comprobar si hay datos disponibles en stdin
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            char input[100];
            fgets(input, sizeof(input), stdin);
            printf("Entrada del usuario: %s", input);
        }
    }

    close(sockfd);
    return 0;
}