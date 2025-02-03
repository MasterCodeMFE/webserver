/* 
    La función socketpair se utiliza para crear un par de sockets conectados, que permiten la
    comunicación bidireccional entre procesos en sistemas operativos Unix y Linux. Estos sockets son
    útiles para la comunicación entre procesos que están relacionados, como un proceso padre y un
    proceso hijo, y se comunican a través de un canal de comunicación interno en lugar de a través de la
    red.

    Prototipo de la función:
        int socketpair(int domain, int type, int protocol, int sv[2]);

        Parámetros:
            domain: Especifica el dominio del socket. Generalmente se utiliza AF_UNIX para sockets de dominio Unix.
            type: Especifica el tipo de socket. Puede ser SOCK_STREAM para un socket orientado a la conexión o SOCK_DGRAM para un socket sin conexión.
            protocol: Generalmente se establece en 0, lo que indica que se debe elegir el protocolo por defecto para el tipo de socket especificado.
            sv: Un arreglo de dos enteros donde se almacenan los descriptores de los dos sockets conectados.

        Valor de retorno:
            Devuelve 0 en caso de éxito.
            Devuelve -1 en caso de error y establece errno para indicar el error.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

int main() {
    int sv[2]; // Array para almacenar los descriptores de los sockets
    pid_t pid;

    // Crear un par de sockets
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("Error al crear socketpair");
        exit(EXIT_FAILURE);
    }

    // Crear un nuevo proceso
    pid = fork();

    if (pid < 0) { // Error en la creación del proceso
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Proceso hijo
        close(sv[0]); // Cerrar el socket no utilizado
        const char *msg = "Hola desde el proceso hijo";
        write(sv[1], msg, strlen(msg)); // Enviar mensaje al padre
        close(sv[1]); // Cerrar el socket
    } else { // Proceso padre
        close(sv[1]); // Cerrar el socket no utilizado
        char buffer[100];
        read(sv[0], buffer, sizeof(buffer)); // Leer mensaje del hijo
        printf("El proceso padre recibió: %s\n", buffer);
        close(sv[0]); // Cerrar el socket
    }

    return 0;
}