/* 
    La función close se usa para cerrar un descriptor de archivo en sistemas UNIX y Linux. Libera los
    recursos asociados al descriptor y permite que otros procesos reutilicen ese número de descriptor.

    Prototipo:
        #include <unistd.h>

        int close(int fd);

        Parámetros:
            fd: Descriptor de archivo que se desea cerrar.
        Valor de retorno:
            Devuelve 0 si tiene éxito.
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

    printf("Archivo abierto con descriptor %d\n", fd);

    // Cerrar el archivo
    if (close(fd) == -1) {
        perror("Error al cerrar el archivo");
        return 1;
    }

    printf("Archivo cerrado correctamente.\n");

    return 0;
}