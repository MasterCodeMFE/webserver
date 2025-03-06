/* 
    La función strerror se utiliza para obtener una descripción en forma de cadena del error
    correspondiente a un código de error dado, generalmente establecido por funciones de llamada al
    sistema o funciones de biblioteca en C. Esta función es útil para proporcionar mensajes de error
    legibles para el usuario cuando se produce un error.

    Prototipo de la función:
        char *strerror(int errnum);

        Parámetros:
            errnum: Un entero que representa el código de error para el cual se desea obtener una
            descripción.
        Valor de retorno:
            Devuelve un puntero a una cadena que describe el error correspondiente al errnum.
            La cadena devuelta puede ser una cadena estática, por lo que no debe ser modificada.

 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

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