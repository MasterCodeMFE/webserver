/* 
    La función poll se utiliza para monitorear múltiples descriptores de archivo (como sockets,
    archivos, etc.) para verificar si están listos para realizar operaciones de lectura, escritura o si hay
    excepciones. Al igual que select, permite a una aplicación manejar múltiples conexiones, pero
    poll puede ser más eficiente y escalable cuando se trabaja con un gran número de descriptores, ya
    que no tiene las limitaciones de tamaño de conjunto que tiene select.

    Prototipo de la función:
        #include <poll.h>

        int poll(struct pollfd *fds, nfds_t nfds, int timeout);

        Parámetros:
            fds: Un puntero a un arreglo de estructuras pollfd, que especifican los descriptores de archivo a monitorear y los eventos de interés.
            nfds: El número de elementos en el arreglo fds.
            timeout: El tiempo de espera en milisegundos. Si se establece en 0, la función retorna
            inmediatamente. Si se establece en -1, poll bloqueará indefinidamente hasta que se produzca
            un evento. Un valor positivo especifica un tiempo de espera en milisegundos.
        Valor de retorno:
            Retorna el número de descriptores de archivo que están listos para la operación, o 0 si se
            alcanza el tiempo de espera, o -1 si ocurre un error.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    struct pollfd fds[2];
    int timeout;

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

    // Configurar el arreglo de pollfd
    fds[0].fd = sockfd; // Socket del servidor
    fds[0].events = POLLIN; // Interesado en eventos de lectura

    fds[1].fd = STDIN_FILENO; // Entrada estándar
    fds[1].events = POLLIN; // Interesado en eventos de lectura

    timeout = 5000; // Tiempo de espera de 5 segundos

    printf("Esperando por datos...\n");

    // Monitorear los descriptores de archivo
    int ret = poll(fds, 2, timeout);
    if (ret < 0) {
        perror("Error en poll");
        close(sockfd);
        return 1;
    } else if (ret == 0) {
        printf("Tiempo de espera alcanzado. No hay actividad.\n");
    } else {
        // Comprobar si hay datos disponibles en el socket
        if (fds[0].revents & POLLIN) {
            char buffer[1024];
            ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0'; // Asegurarse de que sea una cadena
                printf("Datos recibidos del servidor: %s\n", buffer);
            }
        }

        // Comprobar si hay datos disponibles en stdin
        if (fds[1].revents & POLLIN) {
            char input[100];
            fgets(input, sizeof(input), stdin);
            printf("Entrada del usuario: %s", input);
        }
    }

    close(sockfd);
    return 0;
}