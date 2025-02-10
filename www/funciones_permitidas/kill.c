/* 
    La función kill se usa para enviar una señal a un proceso o a un grupo de procesos. No
    necesariamente termina el proceso; depende de la señal enviada.

    Prototipo:
        #include <sys/types.h>
        #include <signal.h>

        int kill(pid_t pid, int sig);

        Parámetros:
            pid:
                > 0: Envía la señal al proceso con ese PID.
                0: Envía la señal a todos los procesos en el mismo grupo del proceso que la llama.
                -1: Envía la señal a todos los procesos que el usuario tiene permiso para matar.
                < -1: Envía la señal a todos los procesos en el grupo de procesos |pid| (valor absoluto de pid).
            sig: La señal que se enviará. Algunas señales comunes:
                SIGKILL (9): Termina el proceso inmediatamente.
                SIGTERM (15): Solicita al proceso que termine.
                SIGSTOP (19): Pausa el proceso.
                SIGCONT (18): Continúa un proceso pausado.
        Valor de retorno:
            0 si la señal se envió correctamente.
            -1 en caso de error y establece errno.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
        return 1;
    }

    if (pid == 0) {
        // Código del proceso hijo
        printf("Soy el hijo con PID %d, esperando señal...\n", getpid());
        while (1) {
            sleep(1); // Simula que está ejecutándose
        }
    } else {
        // Código del proceso padre
        sleep(3); // Espera antes de matar al hijo
        printf("Padre enviando SIGKILL al hijo %d\n", pid);
        kill(pid, SIGKILL); // Termina el proceso hijo
    }

    return 0;
}