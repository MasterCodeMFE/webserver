/* 
    errno es una variable global en C que se utiliza para indicar el código de error que se produce
    cuando una función de la biblioteca estándar o una llamada al sistema falla. Es un entero que se
    establece a un valor específico que representa el tipo de error ocurrido, permitiendo a los
    programadores identificar y manejar errores de manera más efectiva.

    Prototipo de uso:
        extern int errno;

        Descripción:
            errno: No se declara como una función, sino como una variable global de tipo entero. Se puede
            incluir en el código simplemente incluyendo el encabezado <errno.h>.
            Después de que se llama a una función que puede fallar (como open, malloc, fopen, etc.), se
            puede comprobar el valor de errno para determinar qué tipo de error ha ocurrido.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
    // Intentar abrir un archivo que no existe
    FILE *file = fopen("archivo_inexistente.txt", "r");
    if (file == NULL) {
        // Si fopen falla, se imprime el mensaje de error correspondiente
        printf("Error al abrir el archivo: %s\n", strerror(errno));
        return 1;
    }

    // Cerrar el archivo si se abrió correctamente
    fclose(file);
    return 0;
}