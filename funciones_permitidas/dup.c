/* 
    La función dup es una llamada al sistema en Unix y Linux que se utiliza para duplicar un descriptor
    de archivo. Crea un nuevo descriptor que se refiere al mismo archivo o recurso que el descriptor
    original. Esto es útil para redirigir la entrada y salida de programas.

    Prototipo de la función:
        int dup(int oldfd);
        
        Parámetros:
            oldfd: Un descriptor de archivo existente que se desea duplicar.
        Valor de retorno:
            Si tiene éxito, dup devuelve un nuevo descriptor de archivo que es el menor número de descriptor no utilizado.
            Si falla, retorna -1 y establece errno para indicar el error.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    // Abrir un archivo para escritura
    int fd = open("archivo.txt", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Duplicar el descriptor de archivo
    int newfd = dup(fd);
    if (newfd == -1) {
        perror("Error al duplicar el descriptor de archivo");
        close(fd);
        return 1;
    }

    // Usar el nuevo descriptor (newfd) para escribir en el mismo archivo
    write(newfd, "Hola, mundo\n", 12);

    // Cerrar los descriptores
    close(fd);
    close(newfd);

    return 0;
}