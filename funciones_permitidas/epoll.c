/* 
    epoll es una función del sistema de Linux que permite manejar eventos de entrada/salida en
    múltiples descriptores de archivo de manera eficiente. Es similar a poll y select, pero está
    diseñada para manejar un gran número de descriptores de archivo, proporcionando mejor
    rendimiento y escalabilidad. epoll permite a los programas monitorizar múltiples sockets, pipes, y
    otros tipos de descriptores de archivo sin las limitaciones de tamaño de select.

    Prototipo de la función:
        #include <sys/epoll.h>

        int epoll_create(int size);
        int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
        int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

        Parámetros:
            epoll_create:
                size: Aproximadamente el número de descriptores de archivo que se espera que se
            epoll_ctl:
                epfd: El descriptor de archivo devuelto por epoll_create.
                op: La operación a realizar (EPOLL_CTL_ADD, EPOLL_CTL_MOD, EPOLL_CTL_DEL).
                fd: El descriptor de archivo a controlar.
                event: Un puntero a una estructura epoll_event que especifica el tipo de eventos a monitorear.
            epoll_wait:
                epfd: El descriptor de archivo devuelto por epoll_create.
                events: Un puntero a un arreglo de epoll_event donde se almacenarán los eventos listos.
                maxevents: El número máximo de eventos a devolver.
                timeout: El tiempo de espera en milisegundos. Un valor de -1 significa que espera indefinidamente.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

struct epoll_event {
    uint32_t events; // Eventos a monitorizar (ej. EPOLLIN, EPOLLOUT)
    epoll_data_t data; // Datos asociados al descriptor de archivo
};

int main() {
    int sockfd, new_sockfd, epfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    struct epoll_event event, events[MAX_EVENTS];

    // Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la dirección del servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Vincular el socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al vincular el socket");
        close(sockfd);
        return 1;
    }

    // Escuchar conexiones
    listen(sockfd, 5);

    // Crear el epoll
    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("Error al crear epoll");
        close(sockfd);
        return 1;
    }

    // Añadir el socket al epoll
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
        perror("Error al añadir socket al epoll");
        close(sockfd);
        return 1;
    }

    printf("Esperando conexiones...\n");

    while (1) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("Error en epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == sockfd) {
                // Aceptar nueva conexión
                client_len = sizeof(client_addr);
                new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
                if (new_sockfd == -1) {
                    perror("Error al aceptar conexión");
                    continue;
                }

                // Añadir el nuevo socket al epoll
                event.events = EPOLLIN;
                event.data.fd = new_sockfd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, new_sockfd, &event) == -1) {
                    perror("Error al añadir nuevo socket al epoll");
                    close(new_sockfd);
                }

                printf("Nueva conexión aceptada\n");
            } else {
                // Manejar datos del cliente
                char buffer[1024];
                ssize_t bytes_received = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    // El cliente se desconectó
                    close(events[i].data.fd);
                    printf("Conexión cerrada\n");
                } else {
                    // Procesar datos recibidos
                    buffer[bytes_received] = '\0'; // Asegurarse de que sea una cadena
                    printf("Datos recibidos: %s\n", buffer);
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
