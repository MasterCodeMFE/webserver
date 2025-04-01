/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPolling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:08:05 by manufern          #+#    #+#             */
/*   Updated: 2025/03/28 10:55:08 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// 3️⃣ Poner el socket en modo de escucha

#include "DeployServer.hpp"

// ========================================
//  FUNCIÓN: run_server_event_loop
// ========================================
// Función principal que maneja las conexiones del servidor.
//
// Se encarga de:
// 1. Configurar los sockets de escucha.
// 2. Entrar en el bucle principal de eventos (`poll`).
// 3. Procesar los eventos de los servidores y clientes.
//
// Parámetros:
// - server_fds: Vector de descriptores de socket de los servidores.
// - config: Configuración general.
//
// Retorno:
// - 0 si todo se ejecuta correctamente.
// - -1 en caso de error.
int DeployServer::_run_server_event_loop( void )
{
    // Configurar los sockets en modo de escucha y añadirlos a `pollfd`
    if (this->_setup_server_listeners() == -1)
        return -1;

    while (!g_signal )
    {
        // Esperar eventos en los sockets registrados
        int ret = poll(this->fds.data(), this->fds.size(), -1);
        if (ret == -1 && !g_signal)
        {
            std::cerr << "[ERROR] Error en poll(): " << strerror(errno) << std::endl;
            return -1;
        }

        // Procesar eventos de los servidores
        this->_process_server_events( );

        // Procesar eventos de los clientes conectados
        this->_process_client_events(server_fds.size() );
    }
    return 0;
}

// ========================================
//  FUNCIÓN: setup_server_listeners
// ========================================
// Configura el modo de escucha para cada socket de servidor y los agrega
// al vector de `pollfd` para la monitorización de eventos.
//
// Parámetros:
// - server_fds: Vector de descriptores de socket de los servidores.
// - config: Configuración que contiene información de cada servidor.
// - fds: Vector donde se agregarán los `pollfd` configurados.
//
// Retorno:
// - 0 si todo fue bien.
// - -1 en caso de error.
int DeployServer::_setup_server_listeners(void )
{
    listenSet::const_iterator listenit = this->listeners.begin();

    for (size_t i = 0; i < this->server_fds.size(); i++)
    {
        if (listen(this->server_fds[i], SOMAXCONN) == -1)
        {
            std::cerr << "[ERROR] No se pudo poner en escucha el socket en "
                      << listenit->first << " ("
                      << listenit->second << "): "
                      << strerror(errno) << std::endl;
            return -1;
        }

        std::cout << "[INFO] Servidor en escucha en "
                  << listenit->second
                  << ":" << listenit->first
                  << std::endl;

        // Configurar el socket en poll para monitorizar eventos de entrada y salida.
        pollfd server_pollfd = {};
        server_pollfd.fd = this->server_fds[i];
        server_pollfd.events = POLLIN | POLLOUT;
        this->fds.push_back(server_pollfd);
        listenit ++;
    }
    return 0;
}

// ========================================
//  FUNCIÓN: process_server_events
// ========================================
// Procesa los eventos de los sockets de servidor.
//
// Si se detecta un evento de lectura (`POLLIN`), se acepta la conexión
// y se agrega el nuevo socket de cliente al vector `fds`.
//
// Parámetros:
// - server_fds: Vector de descriptores de socket de los servidores.
// - config: Configuración general.
// - fds: Vector de `pollfd` donde se registran los sockets (tanto de servidores como de clientes).
void    DeployServer::_process_server_events( void )
{
    for (size_t i = 0; i < this->server_fds.size(); i++)
    {
        if (this->fds[i].revents & POLLIN)
        {
            int new_client_fd = this->_accept_client_connection(this->server_fds[i]);
            if (new_client_fd != -1)
            {
                // Se configura el nuevo socket de cliente en `poll`
                pollfd new_pollfd = {};
                new_pollfd.fd = new_client_fd;
                new_pollfd.events = POLLIN | POLLOUT;
                this->fds.push_back(new_pollfd);
            }
        }
    }
}

// ========================================
//  FUNCIÓN: process_client_events
// ========================================
// Procesa los eventos de los sockets correspondientes a clientes.
//
// Si un cliente se desconecta (`handle_client_request` retorna -1),
// se cierra el socket y se elimina del vector.
//
// Parámetros:
// - start_index: Índice desde el cual inician los sockets de cliente en el vector `fds`.
// - config: Configuración general.
// - fds: Vector de `pollfd` que contiene tanto servidores como clientes.
void DeployServer::_process_client_events(size_t start_index)
{
    for (size_t i = start_index; i < this->fds.size(); ++i)
    {
        // Verificar si hay eventos en el socket del cliente
        if (this->fds[i].revents & (POLLIN | POLLHUP | POLLERR))
        {
            // Solo procesar si hay datos disponibles
            if (this->fds[i].revents & POLLIN)
            {
                int res = this->_handle_client_request(this->fds[i].fd);
                if (res == -1)  // Cliente desconectado
                {
                    std::cout << "[INFO] Cliente desconectado: " << this->fds[i].fd << "\n";
                    close(this->fds[i].fd);
                    this->fds.erase(this->fds.begin() + i);
                    --i; // Ajustar índice tras la eliminación
                }
            }
        }
    }
}


