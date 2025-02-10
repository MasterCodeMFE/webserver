/* 
    La función access se usa para verificar si un archivo o directorio existe y si el proceso tiene ciertos
    permisos sobre él, como lectura, escritura o ejecución.

    Prototipo:
        #include <unistd.h>

        int access(const char *pathname, int mode);

        Parámetros:
            pathname: Ruta del archivo o directorio a comprobar.
            mode: Modo de acceso que se quiere verificar. Puede ser:
                F_OK → Verifica si el archivo existe.
                R_OK → Verifica si se tiene permiso de lectura.
                W_OK → Verifica si se tiene permiso de escritura.
                X_OK → Verifica si se tiene permiso de ejecución.
        Valor de retorno:
            0 → Si se tienen los permisos solicitados.
            -1 → Si no se tienen los permisos o el archivo no existe (y errno se establece con el error).
 */

#include <stdio.h>
#include <unistd.h>

int main() {
    const char *filename = "archivo.txt";

    // Verificar si el archivo existe
    if (access(filename, F_OK) == 0) {
        printf("El archivo '%s' existe.\n", filename);

        // Verificar permisos de lectura
        if (access(filename, R_OK) == 0) {
            printf("Se puede leer '%s'.\n", filename);
        } else {
            printf("No se puede leer '%s'.\n", filename);
        }

        // Verificar permisos de escritura
        if (access(filename, W_OK) == 0) {
            printf("Se puede escribir en '%s'.\n", filename);
        } else {
            printf("No se puede escribir en '%s'.\n", filename);
        }
    } else {
        printf("El archivo '%s' no existe.\n", filename);
    }

    return 0;
}