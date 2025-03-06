/* 
    La función fork se utiliza en sistemas operativos Unix y Linux para crear un nuevo proceso
    duplicando el proceso que la llama. El nuevo proceso se llama "proceso hijo" y es una copia casi
    exacta del proceso padre. Esta función es fundamental para la programación concurrente, ya que
    permite que un programa ejecute múltiples tareas simultáneamente.

    Prototipo de la función:
        pid_t fork(void);
        
        Valor de retorno:
            En el proceso padre: fork devuelve el ID del proceso hijo (un número entero positivo).
            En el proceso hijo: fork devuelve 0.
            Si ocurre un error: fork devuelve -1, y errno se establece para indicar el error.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    // Crear un nuevo proceso
    pid = fork();

    if (pid < 0) { // Error en la creación del proceso
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Proceso hijo
        printf("Soy el proceso hijo con PID: %d\n", getpid());
        // El proceso hijo puede ejecutar tareas diferentes aquí
    } else { // Proceso padre
        printf("Soy el proceso padre con PID: %d y el PID de mi hijo es: %d\n", getpid(), pid);
        // El proceso padre puede realizar tareas diferentes aquí
    }

    return 0;
}
