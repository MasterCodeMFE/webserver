/* 
    La función dup2 es una llamada al sistema en Unix y Linux que duplica un descriptor de archivo,
    redirigiendo un descriptor de archivo existente hacia un nuevo descriptor especificado. Si el
    descriptor de destino ya está abierto, dup2 lo cierra antes de redirigirlo. Esto es útil para redirigir la
    entrada y salida de programas a descriptores específicos, como la entrada/salida estándar.

    Prototipo de la función:
        int dup2(int oldfd, int newfd);
        
        Parámetros:
            oldfd: El descriptor de archivo que se desea duplicar.
            newfd: El descriptor de archivo al que se redirigirá oldfd. Si newfd ya está abierto, se cerrará
            antes de la redirección.
        Valor de retorno:
            Si tiene éxito, dup2 devuelve newfd.
            Si falla, retorna -1 y establece errno para indicar el error.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    // Abrir un archivo para escritura
    int fd = open("archivo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Redirigir la salida estándar (1) al archivo
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Error al redirigir stdout");
        close(fd);
        return 1;
    }

    // Ahora cualquier salida que se envíe a stdout se escribirá en archivo.txt
    printf("Este texto se escribirá en el archivo.\n");

    // Cerrar el descriptor original
    close(fd);

    return 0;
}