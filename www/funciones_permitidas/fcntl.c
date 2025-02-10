/* 
    La función fcntl se utiliza para manipular descriptores de archivo en sistemas Unix y Linux. Permite
    cambiar propiedades de los descriptores, como configurarlos en modo no bloqueante, duplicarlos o gestionar bloqueos de archivos.

    Prototipo:
        #include <fcntl.h>
        #include <unistd.h>

        int fcntl(int fd, int cmd, ... );

        Parámetros:
            fd: Descriptor de archivo sobre el cual se aplicará la operación.
            cmd: Comando que indica la acción a realizar. Algunos valores comunes son:
                F_GETFL: Obtiene las banderas del descriptor de archivo.
                F_SETFL: Establece nuevas banderas en el descriptor de archivo.
                F_DUPFD: Duplica el descriptor de archivo.
                F_SETLK: Establece un bloqueo en el archivo.
                F_GETFD: Obtiene las banderas del descriptor de archivo.
            argumento opcional: Dependiendo del comando cmd, puede ser necesario un tercer argumento.
        Valor de retorno:
            Si tiene éxito, devuelve un valor no negativo.
            En caso de error, devuelve -1 y establece errno.
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

    // Obtener los flags actuales del descriptor de archivo
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("Error al obtener flags");
        close(fd);
        return 1;
    }

    // Establecer el modo no bloqueante
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("Error al establecer modo no bloqueante");
        close(fd);
        return 1;
    }

    printf("El archivo está en modo no bloqueante.\n");

    close(fd);
    return 0;
}