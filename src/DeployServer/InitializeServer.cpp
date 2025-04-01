/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitializeServer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:52 by manufern          #+#    #+#             */
/*   Updated: 2025/03/20 14:04:16 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// 1️⃣ Crear el socket

#include "DeployServer.hpp"

static int setup_server_socket( std::pair<std::string, std::string> const &listener );
static int create_and_configure_socket( void );
static void close_socket(int server_fd);

// ========================================
//  FUNCIÓN: initialize_server_sockets
// ========================================
// Función principal que coordina la inicialización y gestión de los servidores.
//
// Parámetros:
// - config: Configuración del servidor.
//
// Retorno:
// - 0 si la inicialización y ejecución fueron exitosas.
// - 1 si hubo algún error.
//
// Este flujo sigue tres pasos:
// 1. Se inicializan los sockets de los servidores.
// 2. Se llama a `run_server_event_loop()` para manejar las conexiones de los clientes.
// 3. Se cierran todos los sockets al finalizar.
int DeployServer::initialize_server_sockets( void ) {
	// Inicializa los sockets de todos los servidores según la configuración.
	this->_init_server_sockets( );
	if (server_fds.empty()) { // Si no se pudo configurar ningún socket, se reporta un error.
		std::cerr << "No se pudieron inicializar los sockets del servidor." << std::endl;
		return 1; // Se retorna error.
	}

	// Se inicia el bucle de eventos para manejar múltiples conexiones de clientes.
	// Se asume que `run_server_event_loop` utiliza `poll()`, `select()`, o un mecanismo similar.
	if (this->_run_server_event_loop() == -1) {
		// Si hay un error durante la ejecución del servidor, se cierran los sockets abiertos.
		for (size_t i = 0; i < server_fds.size(); i++) {
			close_socket(server_fds[i]);
		}
		return 1;
	}

	// Si el servidor finaliza correctamente, se cierran todos los sockets antes de salir.
	for (size_t i = 0; i < server_fds.size(); i++) {
		close_socket(server_fds[i]);
	}
	return 0; // Retorna éxito.
}

// ========================================
//  FUNCIÓN: init_server_sockets
// ========================================
// Inicializa los sockets de todos los servidores definidos en la configuración.
//
// Retorno:
// - Vector con los descriptores de los sockets configurados.
// - Si no se pudo inicializar ningún socket, devuelve un vector vacío.
//
// Esta función recorre la lista de servidores en `config.getVServers()` y configura un socket para cada uno.
DeployServer	&DeployServer::_init_server_sockets(void) {
	int server_fd;

	// Utiliza un iterador no constante.
	for (listenSet::iterator it = this->listeners.begin(); it != this->listeners.end();  ) {
		server_fd = setup_server_socket(*it);
		if (server_fd != -1) { // Si el socket se configuró correctamente, se agrega al vector.
		{
			this->server_fds.push_back(server_fd); // Avanzar al siguiente iterador solo si se configura correctamente.
			it++;
		}
		} else {
			std::cerr << "No se pudo configurar el socket para el servidor `" << it->second << "` (" << it->first << ")" << std::endl;
			// Aquí usamos erase para eliminar el elemento y actualizamos el iterador.
			listenSet::iterator tmp = it;
			tmp ++;
			this->listeners.erase(it);
			it = tmp;                       
		}
	}

	for (listenSet::const_iterator it = listeners.begin(); it != listeners.end(); ++it)
		std::cout << "listening: " << it->first << " server name: " << it->second << std::endl;

	return *this; // Se devuelve el objeto actual.
}

// ========================================
//  FUNCIÓN: setup_server_socket
// ========================================
// Configura un socket para un servidor específico.
//
// Parámetros:
// - config: Objeto que contiene la configuración del servidor.
// - index: Índice del servidor en la configuración.
//
// Retorno:
// - Descriptor del socket configurado si tiene éxito.
// - -1 si ocurre un error.
//
// Esta función realiza dos pasos:
// 1. Crea y configura un socket con `create_and_configure_socket()`.
// 2. Llama a `configure_and_bind_socket()` para enlazarlo y prepararlo.
static int setup_server_socket( std::pair<std::string, std::string> const &listener )
{
	// Se crea el socket para el servidor en la posición `index`.
	int server_fd = create_and_configure_socket();
	if (server_fd == -1) { // Si no se pudo crear el socket, se devuelve error.
		std::cerr << "Error creando socket para servidor `" << listener.second 
			<< "`(" << listener.first << ")"<<  std::endl;
		return -1;
	}

	// `configure_and_bind_socket` debe encargarse de hacer bind() y posiblemente listen().
	if ( configure_and_bind_socket(server_fd, listener) == -1)
	{
		// meter funcion que borre el server que falla
		close_socket(server_fd); // Si la configuración falla, se cierra el socket.
		return -1;
	}
	return server_fd; // Retorna el descriptor del socket correctamente configurado.
}

// ========================================
//  FUNCIÓN: create_and_configure_socket
// ========================================
// Crea un socket TCP y le asigna la opción SO_REUSEADDR.
//
// Retorno:
// - Descriptor de archivo del socket si la creación y configuración tienen éxito.
// - -1 si ocurre un error.
//
// SO_REUSEADDR permite reutilizar una dirección y puerto sin esperar el timeout del SO.
// Esto es útil en servidores que pueden reiniciarse rápidamente.
static int create_and_configure_socket()
{
	// Se crea un socket de dominio IPv4 (AF_INET) y tipo TCP (SOCK_STREAM).
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) { // Verificar si hubo error en la creación del socket.
		std::cerr << "Error al crear el socket." << std::endl;
		return -1;
	}
	// Configurar la opción SO_REUSEADDR.
	// Esto evita problemas al reiniciar el servidor, permitiendo reutilizar la dirección de inmediato.
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << "Error al configurar SO_REUSEADDR." << std::endl;
		close_socket(server_fd); // Cerrar el socket si la configuración falla.
		return -1;
	}
	return server_fd; // Retornar el descriptor del socket configurado.
}


// ========================================
//  FUNCIÓN: close_socket
// ========================================
// Cierra el socket especificado y libera los recursos asociados.
//
// Parámetros:
// - server_fd: Descriptor del socket a cerrar.
//
// Esta función es importante para evitar fugas de recursos en el sistema operativo.
static void close_socket(int server_fd)
{
	close(server_fd); // Llamada al sistema para cerrar el socket.
}

