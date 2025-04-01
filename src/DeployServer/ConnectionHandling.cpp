/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionHandling.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:27 by manufern          #+#    #+#             */
/*   Updated: 2025/03/06 19:07:28 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// 4️⃣ Aceptar una conexión entrante

#include "DeployServer.hpp"

static int acceptConnection(int server_fd);
static void setNonBlocking(int client_fd);
static void enableKeepAlive(int client_fd);

// ========================================
//  FUNCIÓN: configureKeepAliveTimes
// ========================================
// Configura los parámetros de keep-alive del socket.
//
// Se establecen tres valores:
// 1. TCP_KEEPIDLE: Tiempo en segundos antes de enviar el primer keep-alive.
// 2. TCP_KEEPINTVL: Intervalo en segundos entre cada keep-alive.
// 3. TCP_KEEPCNT: Número de intentos antes de considerar la conexión caída.
//
// Parámetros:
// - client_fd: Descriptor del socket del cliente.
// - keep_idle: Tiempo antes del primer keep-alive (por defecto 5 seg).
// - keep_intvl: Intervalo entre keep-alives (por defecto 5 seg).
// - keep_cnt: Número de intentos antes de dar por caída la conexión (por defecto 3).
//
// Retorno:
// - Ninguno.
static void configureKeepAliveTimes(int client_fd, int keep_idle = 5, int keep_intvl = 5, int keep_cnt = 3)
{
    if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPIDLE, &keep_idle, sizeof(keep_idle)) < 0) {
        std::cerr << "[ERROR] No se pudo configurar TCP_KEEPIDLE: " << strerror(errno) << std::endl;
    }
    if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPINTVL, &keep_intvl, sizeof(keep_intvl)) < 0) {
        std::cerr << "[ERROR] No se pudo configurar TCP_KEEPINTVL: " << strerror(errno) << std::endl;
    }
    if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPCNT, &keep_cnt, sizeof(keep_cnt)) < 0) {
        std::cerr << "[ERROR] No se pudo configurar TCP_KEEPCNT: " << strerror(errno) << std::endl;
    }
}

// ========================================
//  FUNCIÓN: accept_client_connection
// ========================================
// Maneja la aceptación y configuración inicial de un cliente.
//
// Se encarga de:
// 1. Aceptar una nueva conexión en `server_fd`.
// 2. Configurar el socket en modo no bloqueante.
// 3. Activar keep-alive y establecer los parámetros de keep-alive.
//
// Parámetros:
// - server_fd: Descriptor del socket del servidor.
// - config: Configuración del servidor (no utilizada en este código).
//
// Retorno:
// - Descriptor del socket del cliente si la conexión es exitosa.
// - -1 en caso de error.
int DeployServer::_accept_client_connection(int server_fd ) 
{
    (void)this->locations; // `config` se ignora en este ejemplo, pero puede usarse en futuras mejoras.

    int client_fd = acceptConnection(server_fd);
    if (client_fd == -1) {
        return -1;
    }

    setNonBlocking(client_fd);
    enableKeepAlive(client_fd);
    configureKeepAliveTimes(client_fd);

    return client_fd;
}

// ========================================
//  FUNCIÓN: acceptConnection
// ========================================
// Acepta una nueva conexión de un cliente en el servidor.
//
// Se encarga de:
// 1. Esperar y aceptar una conexión entrante en `server_fd`.
// 2. Obtener el descriptor de socket del cliente.
// 3. Imprimir un mensaje en caso de éxito o error.
//
// Parámetros:
// - server_fd: Descriptor del socket del servidor.
//
// Retorno:
// - Descriptor del socket del cliente en caso de éxito.
// - -1 si ocurre un error.
static int acceptConnection(int server_fd)
{
    sockaddr_in client_addr;                  
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
    
    if (client_fd == -1) {
        std::cerr << "[ERROR] No se pudo aceptar la conexión: " << strerror(errno) << std::endl;
    } else {
        std::cout << "[INFO] Nuevo cliente conectado: " << client_fd << "\n";
    }
    return client_fd;
}

// ========================================
//  FUNCIÓN: setNonBlocking
// ========================================
// Configura el socket del cliente en modo no bloqueante.
//
// Esto permite que las operaciones de lectura y escritura en el socket
// no se bloqueen si no hay datos disponibles.
//
// Parámetros:
// - client_fd: Descriptor del socket del cliente.
//
// Retorno:
// - Ninguno.
static void setNonBlocking(int client_fd)
{
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "[ERROR] No se pudo configurar el socket como no bloqueante: " 
                  << strerror(errno) << std::endl;
    }
}

// ========================================
//  FUNCIÓN: enableKeepAlive
// ========================================
// Activa la opción SO_KEEPALIVE en el socket del cliente.
//
// Esto permite detectar desconexiones inactivas enviando paquetes de prueba.
//
// Parámetros:
// - client_fd: Descriptor del socket del cliente.
//
// Retorno:
// - Ninguno.
static void enableKeepAlive(int client_fd)
{
    int optval = 1;
    if (setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0) {
        std::cerr << "[ERROR] No se pudo habilitar keep-alive: " << strerror(errno) << std::endl;
    }
}
