/* 
    1. Crear el socket	Crear un socket TCP que permita conexiones.	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    2. Configurar opciones	Configurar las opciones del socket, como permitir reutilizar la dirección del puerto.	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    3. Configurar dirección	Establecer la estructura de dirección que especifica la familia de direcciones y el puerto.	address.sin_family = AF_INET; address.sin_addr.s_addr = INADDR_ANY; address.sin_port = htons(PORT);
    4. Vincular el socket	Vincular el socket a la dirección y puerto especificados.	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    5. Escuchar conexiones	Poner el socket en modo de escucha para aceptar conexiones entrantes.	listen(server_fd, 3);
    6. Aceptar conexiones	Aceptar una nueva conexión y crear un nuevo socket para el cliente.	new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    7. Leer la solicitud	Leer los datos enviados por el cliente (solicitud HTTP).	read(new_socket, buffer, 1024);
    8. Procesar la solicitud	Analizar la solicitud (por ejemplo, extraer el método y la ruta) y determinar la respuesta.	std::cout << "Solicitud recibida:\n" << buffer << std::endl;
    9. Enviar la respuesta	Formar y enviar una respuesta HTTP adecuada al cliente.	send(new_socket, response, strlen(response), 0);
    10. Cerrar la conexión	Cerrar el socket del cliente después de enviar la respuesta.	close(new_socket);
    11. Repetir	Volver al paso 6 para aceptar nuevas conexiones.	while (true) { ... }
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt,)
}