/* 
    La función open se utiliza para abrir un archivo y obtener un descriptor de archivo. Permite
    especificar el modo de acceso (lectura, escritura, o ambos) y si se debe crear el archivo si no existe.

    Prototipo:
        #include <fcntl.h>
        #include <unistd.h>

        int open(const char *pathname, int flags, mode_t mode);

        Parámetros:
            pathname: Ruta del archivo que se desea abrir.
            flags: Opciones que determinan el acceso y el comportamiento del archivo. Algunas opciones comunes son:
                O_RDONLY: Abrir el archivo solo para lectura.
                O_WRONLY: Abrir el archivo solo para escritura.
                O_RDWR: Abrir el archivo para lectura y escritura.
                O_CREAT: Crear el archivo si no existe (se requiere mode).
                O_TRUNC: Truncar el archivo a tamaño cero si ya existe.
                O_APPEND: Escribir datos al final del archivo.
            mode: Permisos del archivo, se usa solo si se crea el archivo (usualmente se especifica como 0666 para lectura y escritura para todos).
        Valor de retorno:
            Devuelve un descriptor de archivo (un entero positivo) si tiene éxito.
            Devuelve -1 si ocurre un error (se establece errno con el código de error).
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *filename = "archivo.txt";
    
    // Abrir o crear el archivo para escritura (modo O_WRONLY y O_CREAT)
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    
    if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Escribir en el archivo
    const char *texto = "Hola, mundo!\n";
    if (write(fd, texto, 14) == -1) {
        perror("Error al escribir en el archivo");
        close(fd); // Cerrar el descriptor de archivo antes de salir
        return 1;
    }

    // Cerrar el descriptor de archivo
    close(fd);
    printf("Archivo escrito correctamente.\n");

    return 0;
}