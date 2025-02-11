/* 
    La función getprotobyname se utiliza para obtener información sobre un protocolo de red en base a
    su nombre. Devuelve una estructura de tipo protoent que contiene detalles sobre el protocolo,
    como su número de protocolo y su nombre. Esta función es útil cuando se necesita trabajar con
    protocolos a nivel de socket, como TCP o UDP.

    Prototipo:
        #include <netdb.h>

        struct protoent *getprotobyname(const char *name);

        Parámetros:
            name: Un puntero a una cadena que contiene el nombre del protocolo que se desea buscar (por ejemplo, "tcp", "udp").
        Valor de retorno
            Devuelve un puntero a una estructura protoent que contiene la información del protocolo.
            Si el protocolo no se encuentra, devuelve NULL y se puede usar h_errno para obtener información sobre el error.
 */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

int main() {
    struct protoent *protocol;

    // Obtener información sobre el protocolo TCP
    protocol = getprotobyname("tcp");
    if (protocol == NULL) {
        perror("Error al obtener el protocolo");
        return 1;
    }

    // Imprimir el número de protocolo y el nombre
    printf("Nombre del protocolo: %s\n", protocol->p_name);
    printf("Número del protocolo: %d\n", protocol->p_proto);

    return 0;
}