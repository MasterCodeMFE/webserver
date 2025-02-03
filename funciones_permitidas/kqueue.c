/* 
    kqueue es una interfaz de notificación de eventos disponible en sistemas operativos de tipo BSD,
    como FreeBSD y macOS. Permite a las aplicaciones monitorizar múltiples descriptores de archivo y
    eventos de kernel, como cambios en el estado de sockets, archivos y otros recursos. kqueue es
    eficiente y está diseñado para manejar un gran número de eventos con bajo consumo de recursos.

    Prototipo de la función:
        #include <sys/event.h>
        #include <sys/time.h>

        int kqueue(void);
        int kevent(int kq, const struct kevent *changes, int nchanges, struct kevent *events, int nevents, const struct timespec *timeout);

        Parámetros:
            kqueue:
                No tiene parámetros.
                Retorna un descriptor de archivo para el nuevo kqueue.
            kevent:
                kq: El descriptor de archivo devuelto por kqueue.
                changes: Un puntero a un arreglo de struct kevent que describe los cambios que se quieren hacer en el kqueue.
                nchanges: El número de eventos a añadir, modificar o eliminar.
                events: Un puntero a un arreglo de struct kevent donde se almacenarán los eventos listos.
                nevents: El número máximo de eventos a devolver.
                timeout: Un puntero a un struct timespec que especifica el tiempo de espera. Si es NULL, espera indefinidamente.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <sys/time.h>

#define PORT 8080
#define MAX_EVENTS 10

struct kevent {
    uintptr_t ident; // Identificador del evento (descriptor de archivo o evento)
    short filter;    // Filtro de evento (ej. EVFILT_READ, EVFILT_WRITE)
    u_short flags;   // Flags de evento (ej. EV_ADD, EV_DELETE, EV_ENABLE)
    u_int fflags;    // Flags adicionales
    intptr_t data;   // Datos específicos del evento
    void *udata;     // Datos de usuario asociados
};

int main() {
    int sockfd, new_sockfd, kq;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    struct kevent changes[MAX_EVENTS], events[MAX_EVENTS];

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
    server_addr.sin_port = htons(PORT);

    // Vincular el socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al vincular el socket");
        close(sockfd);
        return 1;
    }

    // Escuchar conexiones
    listen(sockfd, 5);

    // Crear el kqueue
    kq = kqueue();
    if (kq == -1) {
        perror("Error al crear kqueue");
        close(sockfd);
        return 1;
    }

    // Añadir el socket al kqueue
    EV_SET(&changes[0], sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(kq, changes, 1, NULL, 0, NULL) == -1) {
        perror("Error al añadir socket al kqueue");
        close(sockfd);
        return 1;
    }

    printf("Esperando conexiones...\n");

    while (1) {
        int num_events = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (num_events == -1) {
            perror("Error en kevent");
            break;
        }

        for (int i = 0; i < num_events; i++) {
            if (events[i].ident == sockfd) {
                // Aceptar nueva conexión
                client_len = sizeof(client_addr);
                new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
                if (new_sockfd == -1) {
                    perror("Error al aceptar conexión");
                    continue;
                }

                // Añadir el nuevo socket al kqueue
                EV_SET(&changes[0], new_sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                if (kevent(kq, changes, 1, NULL, 0, NULL) == -1) {
                    perror("Error al añadir nuevo socket al kqueue");
                    close(new_sockfd);
                }

                printf("Nueva conexión aceptada\n");
            } else {
                // Manejar datos del cliente
                char buffer[1024];
                ssize_t bytes_received = recv(events[i].ident, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    // El cliente se desconectó
                    close(events[i].ident);
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