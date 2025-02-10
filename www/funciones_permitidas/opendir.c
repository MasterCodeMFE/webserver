/* 
    La función opendir se utiliza para abrir un directorio y obtener un puntero a un objeto DIR, que se
    puede usar para leer las entradas del directorio utilizando otras funciones como readdir.

    Prototipo:
        #include <dirent.h>

        DIR *opendir(const char *name);

        Parámetros
            name: La ruta del directorio que se desea abrir.
        Valor de retorno:
            Devuelve un puntero a un objeto DIR si tiene éxito.
            Devuelve NULL si ocurre un error (se establece errno con el código de error).
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
        printf("%s\n", entry->d_name); // Imprimir el nombre de cada entrada
    }

    // Cerrar el directorio
    closedir(dir);
    return 0;
}