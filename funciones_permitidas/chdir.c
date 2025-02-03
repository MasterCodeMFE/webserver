/* 
    La función chdir se utiliza para cambiar el directorio de trabajo actual del proceso en ejecución. Al
    invocar esta función, se puede establecer un nuevo directorio en el que se realizarán las operaciones
    de archivo posteriores. Si el directorio especificado no existe o no se puede acceder, la función fallará.

    Prototipo:
        int chdir(const char *path);

        Parámetros:
            path: Una cadena que especifica la ruta del directorio al que se desea cambiar. Puede ser una
            ruta absoluta (comenzando desde la raíz) o una ruta relativa (basada en el directorio actual).
        Valor de retorno:
            Devuelve 0 si el cambio de directorio se realiza con éxito.
            Devuelve -1 en caso de error y establece errno para indicar el tipo de error (como directorio no encontrado o permisos insuficientes).
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
    const char *path = "/home/usuario"; // Cambia a la ruta deseada

    // Cambiar el directorio de trabajo
    if (chdir(path) != 0) {
        // Si chdir falla, imprimir el mensaje de error
        fprintf(stderr, "Error al cambiar de directorio: %s\n", strerror(errno));
        return 1;
    }

    // Mostrar el nuevo directorio de trabajo
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("El nuevo directorio de trabajo es: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }

    return 0;
}