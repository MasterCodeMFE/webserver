/* 
    La función freeaddrinfo se utiliza para liberar la memoria asignada por la función getaddrinfo,
    que resuelve nombres de host y devuelve información de direcciones. Cuando se utiliza
    getaddrinfo, se asigna memoria dinámica para almacenar la estructura addrinfo que contiene
    información sobre las direcciones IP y el tipo de socket. Es importante liberar esta memoria una vez
    que ya no se necesita para evitar fugas de memoria.

    Prototipo:
        void freeaddrinfo(struct addrinfo *res);

        Parámetros:
            res: Un puntero a la estructura addrinfo que se desea liberar. Este puntero debe haber sido devuelto por una llamada anterior a getaddrinfo.
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
        fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    // Usar la información de direcciones (en este caso, solo se muestra)
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        char ipstr[INET6_ADDRSTRLEN];
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("%s: %s\n", ipver, ipstr);
    }

    // Liberar la memoria asignada por getaddrinfo
    freeaddrinfo(res);
    return 0;
}