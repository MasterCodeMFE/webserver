/* 
    La función signal permite establecer un manejador personalizado para una señal específica o
    restaurar su comportamiento por defecto. Se usa para capturar señales como SIGINT (Ctrl+C) o SIGTERM.   

    Prototipo:
        #include <signal.h>

        void (*signal(int signum, void (*handler)(int)))(int);

        Parámetros:
            signum: El número de la señal a capturar. Algunas señales comunes:
                SIGINT (2): Interrupción (Ctrl+C).
                SIGTERM (15): Terminación del proceso.
                SIGKILL (9): No puede ser capturada.
                SIGUSR1, SIGUSR2: Señales definidas por el usuario.
            handler: Puntero a la función que manejará la señal, que debe recibir un int (el número de la señal). Puede ser:
                SIG_IGN: Ignora la señal.
                SIG_DFL: Restaura el comportamiento por defecto.
                Un puntero a una función que maneje la señal.
        Valor de retorno:
            Devuelve el antiguo manejador de la señal si tiene éxito.
            Devuelve SIG_ERR en caso de error.
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Manejador de señal
void handle_sigint(int sig) {
    printf("\nSeñal %d recibida. Presiona Ctrl+C otra vez para salir.\n", sig);
}

int main() {
    // Registrar el manejador para SIGINT
    signal(SIGINT, handle_sigint);

    printf("Presiona Ctrl+C para ver el efecto.\n");
    while (1) {
        sleep(1);
    }

    return 0;
}