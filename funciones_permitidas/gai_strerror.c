/* 
    La función gai_strerror se utiliza para obtener una descripción de error legible en relación con los
    errores que pueden ocurrir durante la resolución de nombres de host y direcciones utilizando la
    familia de funciones de resolución de direcciones de red (como getaddrinfo). Esta función es parte
    de la biblioteca de sockets en C y es útil para manejar errores que se producen durante la resolución
    de nombres.

    Prototipo de la función:
        const char *gai_strerror(int errcode);

        Parámetros:
            errcode: Un entero que representa el código de error devuelto por las funciones de resolución
            de direcciones (por ejemplo, getaddrinfo).
        
        Valor de retorno:
            Devuelve un puntero a una cadena que describe el error correspondiente al errcode. Esta
            cadena es estática y no debe ser modificada.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    struct addrinfo hints, *result;
    int status;

    // Configurar la estructura hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Intentar resolver el nombre de host
    status = getaddrinfo("ejemplo.com", "80", &hints, &result);
    if (status != 0) {
        // Si getaddrinfo falla, imprimir el mensaje de error
        fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    // Si tiene éxito, usar result (no se mostrará aquí para simplificar)
    freeaddrinfo(result); // Liberar la memoria asignada por getaddrinfo
    return 0;
}
