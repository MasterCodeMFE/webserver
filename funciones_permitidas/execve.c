/* 
La función execve es una llamada del sistema en sistemas operativos Unix y Linux que reemplaza el
proceso actual con un nuevo programa. Permite ejecutar un programa diferente dentro del proceso
actual, sin crear un nuevo proceso. Esta función es fundamental para la ejecución de programas en
entornos Unix y se utiliza comúnmente en shells y otros programas que necesitan cargar y ejecutar
otros programas.

Prototipo de la función:
    int execve(const char *pathname, char *const argv[], char *const envp[]);
    
    Parámetros:
        pathname: Una cadena que contiene la ruta al ejecutable que se desea ejecutar.
        argv[]: Una lista de argumentos para el programa, donde el primer elemento es el nombre del programa y el último es NULL.
        envp[]: Una lista de variables de entorno para el nuevo programa, terminada con NULL.
    Valor de retorno:
        Si la llamada es exitosa, execve no retorna, ya que el proceso actual se reemplaza por el nuevo
        programa. Si falla, retorna -1 y establece errno para indicar el error.

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