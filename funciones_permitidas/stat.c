/* 
    La función stat se usa para obtener información sobre un archivo o directorio, como su tamaño,
    permisos, tipo, tiempos de acceso/modificación, etc.

    Prototipo:
        #include <sys/types.h>
        #include <sys/stat.h>
        #include <unistd.h>

        int stat(const char *pathname, struct stat *buf);

        Parámetros:
            pathname: Ruta del archivo o directorio a analizar.
            buf: Puntero a una estructura struct stat donde se almacenará la información obtenida.
        Valor de retorno:
            0 → Si la operación fue exitosa.
            -1 → Si ocurrió un error (se establece errno con el código de error).
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    struct stat fileStat;
    const char *filename = "archivo.txt";

    // Obtener información del archivo
    if (stat(filename, &fileStat) == -1) {
        perror("Error al obtener información del archivo");
        return 1;
    }

    // Imprimir información del archivo
    printf("Tamaño: %ld bytes\n", fileStat.st_size);
    printf("Permisos: %o\n", fileStat.st_mode & 0777); // Máscara para obtener los permisos
    printf("Última modificación: %ld\n", fileStat.st_mtime);

    return 0;
}

