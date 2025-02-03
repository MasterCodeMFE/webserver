/* 
    La función read se usa para leer datos de un descriptor de archivo en sistemas UNIX y Linux. Se
    emplea comúnmente para leer archivos, sockets y entradas estándar.

    Prototipo:
        #include <unistd.h>

        ssize_t read(int fd, void *buf, size_t count);

        Parámetros:
            fd: Descriptor de archivo desde donde se leerán los datos.
            buf: Puntero a un buffer donde se almacenarán los datos leídos.
            count: Número máximo de bytes a leer.
        Valor de retorno:
            Devuelve el número de bytes leídos si tiene éxito.
            Devuelve 0 si se alcanza el final del archivo.
            Devuelve -1 en caso de error y establece errno para indicar el motivo del fallo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("archivo.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }

    char buffer[100];
    ssize_t bytes_leidos = read(fd, buffer, sizeof(buffer) - 1);
    
    if (bytes_leidos == -1) {
        perror("Error al leer el archivo");
        close(fd);
        return 1;
    }

    buffer[bytes_leidos] = '\0'; // Agregar terminador nulo
    printf("Contenido leído:\n%s\n", buffer);

    close(fd);
    return 0;
}