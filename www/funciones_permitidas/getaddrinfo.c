/* 
    La función getaddrinfo se utiliza para resolver nombres de host y direcciones de servicio en
    direcciones de socket. Es una función de la familia de funciones de red que permite obtener
    información sobre una dirección IP y el tipo de socket para conexiones de red, facilitando la creación
    de sockets. Esta función es especialmente útil porque maneja tanto direcciones IPv4 como IPv6 y
    permite la resolución de nombres de dominio.

    Prototipo:
        int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

        Parámetros:
            node: Un puntero a una cadena que representa el nombre del host (por ejemplo, "www.ejemplo.com") o una dirección IP en formato de texto.
            service: Un puntero a una cadena que representa el servicio o puerto (por ejemplo, "80" para HTTP).
            hints: Un puntero a una estructura addrinfo que contiene información sobre las direcciones
            deseadas (por ejemplo, tipo de socket, familia de direcciones).
            res: Un puntero a un puntero que se asignará a una lista de estructuras addrinfo que contienen la información resuelta.
        Valor de retorno:
            Devuelve 0 si la función se ejecuta con éxito.
            Devuelve un código de error diferente de cero en caso de fallo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    struct addrinfo hints, *res;
    int status;

    // Configurar la estructura hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Intentar resolver el nombre de host
    status = getaddrinfo("www.ejemplo.com", "80", &hints, &res);
    if (status != 0) {
        // Si getaddrinfo falla, imprimir el mensaje de error
        fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    // Mostrar la dirección IP resuelta
    char ipstr[INET6_ADDRSTRLEN];
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        // Obtener la dirección
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Convertir la dirección a cadena
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("%s: %s\n", ipver, ipstr);
    }

    // Liberar la memoria asignada por getaddrinfo
    freeaddrinfo(res);
    return 0;
}