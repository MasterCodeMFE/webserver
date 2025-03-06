/* 
    La función waitpid se usa para esperar a que un proceso hijo termine. Permite al proceso padre
    esperar por un hijo específico y obtener su estado de salida.

    Prototipo:
        #include <sys/types.h>
        #include <sys/wait.h>
        #include <unistd.h>

        pid_t waitpid(pid_t pid, int *status, int options);


        Parámetros:
            pid:
                > 0: Espera al proceso con el ID específico.
                -1: Espera a cualquier proceso hijo.
                0: Espera a cualquier hijo del mismo grupo de procesos.
                < -1: Espera a cualquier hijo de un grupo de procesos específico.
            status: Puntero a un int donde se almacenará el estado de salida del hijo.
            options:
                0: Comportamiento normal, bloquea hasta que termine el proceso hijo.
                WNOHANG: No bloquea, retorna inmediatamente si no hay hijos que hayan terminado.
                WUNTRACED: También devuelve hijos que se han detenido.
        Valor de retorno:
            Devuelve el PID del hijo terminado en caso de éxito.
            Devuelve 0 si se usó WNOHANG y el hijo aún no ha terminado.
            Devuelve -1 en caso de error y establece errno.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
        return 1;
    }

    if (pid == 0) {
        // Código del proceso hijo
        printf("Soy el hijo con PID %d\n", getpid());
        sleep(2); // Simula trabajo
        printf("El hijo ha terminado\n");
        exit(42); // Código de salida
    } else {
        // Código del proceso padre
        int status;
        pid_t child_pid = waitpid(pid, &status, 0);

        if (child_pid == -1) {
            perror("Error en waitpid");
            return 1;
        }

        if (WIFEXITED(status)) {
            printf("El hijo %d terminó con código %d\n", child_pid, WEXITSTATUS(status));
        }
    }

    return 0;
}