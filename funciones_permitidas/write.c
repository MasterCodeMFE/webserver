/* 
    La función write se usa para escribir datos en un descriptor de archivo en sistemas UNIX y Linux. Es
    utilizada para escribir en archivos, sockets y salidas estándar.

    Prototipo:
        #include <unistd.h>

        ssize_t write(int fd, const void *buf, size_t count);

        Parámetros:
            fd: Descriptor de archivo donde se escribirán los datos.
            buf: Puntero al buffer que contiene los datos a escribir.
            count: Número de bytes a escribir.
        Valor de retorno:
            Devuelve el número de bytes escritos si tiene éxito.
            Devuelve -1 en caso de error y establece errno para indicar el motivo del fallo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("salida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }

    const char *mensaje = "Hola, mundo!\n";
    ssize_t bytes_escritos = write(fd, mensaje, 14);

    if (bytes_escritos == -1) {
        perror("Error al escribir en el archivo");
        close(fd);
        return 1;
    }

    printf("Se escribieron %zd bytes en el archivo.\n", bytes_escritos);

    close(fd);
    return 0;
}
