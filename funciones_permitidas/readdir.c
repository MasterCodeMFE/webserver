/* 
    La función readdir se utiliza para leer las entradas de un directorio abierto. Cada llamada a
    readdir devuelve un puntero a una estructura dirent que representa la siguiente entrada en el
    directorio. Esta función es comúnmente utilizada después de haber abierto un directorio con opendir.

    Prototipo:
        #include <dirent.h>

        struct dirent *readdir(DIR *dirp);

        Parámetros:
            dirp: Un puntero a un objeto DIR, que representa el directorio que se está leyendo. Este
            puntero debe haber sido devuelto previamente por una llamada a opendir.
        Valor de retorno:
            Devuelve un puntero a una estructura dirent que contiene la información sobre la entrada leída del directorio.
            Devuelve NULL cuando no hay más entradas en el directorio o si ocurre un error.
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
    closedir(dir);
    return 0;
}