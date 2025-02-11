/* 
    La función closedir se utiliza para cerrar un directorio que ha sido abierto con la función opendir.
    Al cerrar un directorio, se liberan los recursos asociados a él. Es importante llamar a closedir
    después de haber terminado de leer un directorio para evitar fugas de memoria.

    Prototipo:
        #include <dirent.h>

        int closedir(DIR *dirp);

        Parámetros:
            dirp: Un puntero a un objeto DIR, que representa el directorio que se desea cerrar. Este
            puntero debe haber sido devuelto previamente por una llamada a opendir.
        Valor de retorno:
            Devuelve 0 si la operación de cierre se realizó con éxito.
            Devuelve -1 si hay un error, en cuyo caso se puede usar errno para obtener más información sobre el error.
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    const char *dir_path = "."; // Ruta del directorio actual

    // Abrir el directorio
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return 1;
    }

    struct dirent *entry;
    // Leer las entradas del directorio
    while ((entry = readdir(dir)) != NULL) {
        // Imprimir el nombre de la entrada
        printf("%s\n", entry->d_name);
    }

    // Cerrar el directorio
    if (closedir(dir) == -1) {
        perror("Error al cerrar el directorio");
        return 1;
    }

    return 0;
}