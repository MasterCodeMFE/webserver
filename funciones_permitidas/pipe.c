/* 
    La función pipe es una llamada al sistema en Unix y Linux que se utiliza para crear un canal de
    comunicación unidireccional entre procesos. Permite que la salida de un proceso se convierta en la
    entrada de otro proceso, facilitando la comunicación entre ellos a través de descriptores de archivo.

    Prototipo de la función:
        int pipe(int pipefd[2]);

        Parámetros:
            pipefd: Un arreglo de dos enteros que se utiliza para almacenar los descriptores de archivo.
            pipefd[0] se utiliza para leer y pipefd[1] se utiliza para escribir.

        Valor de retorno:
            Si tiene éxito, pipe devuelve 0.
            Si falla, retorna -1 y establece errno para indicar el error.
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int pipefd[2];
    int pid;
    char buffer[1024];

    // Crear el pipe
    if (pipe(pipefd) == -1) {
        perror("Error al crear el pipe");
        exit(EXIT_FAILURE);
    }

    // Crear un nuevo proceso
    pid = fork();
    if (pid == -1) {
        perror("Error al hacer fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Proceso hijo
        close(pipefd[1]); // Cerrar el extremo de escritura del pipe
        read(pipefd[0], buffer, sizeof(buffer)); // Leer del pipe
        printf("Proceso hijo recibió: %s\n", buffer);
        close(pipefd[0]); // Cerrar el extremo de lectura del pipe
        exit(EXIT_SUCCESS);
    } else { // Proceso padre
        close(pipefd[0]); // Cerrar el extremo de lectura del pipe
        const char *message = "Hola desde el proceso padre";
        write(pipefd[1], message, strlen(message) + 1); // Escribir en el pipe
        close(pipefd[1]); // Cerrar el extremo de escritura del pipe
        wait(NULL); // Esperar a que el hijo termine
    }

    return 0;
}