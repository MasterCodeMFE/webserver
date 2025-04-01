/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionHandling.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:27 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 20:09:28 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "DeployServer.hpp"

static int acceptConnection(int server_fd);
static void setNonBlocking(int client_fd);
static void enableKeepAlive(int client_fd);

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

int DeployServer::_accept_client_connection(int server_fd ) 
{
    (void)this->locations;

    int client_fd = acceptConnection(server_fd);
    if (client_fd == -1) {
        return -1;
    }

    setNonBlocking(client_fd);
    enableKeepAlive(client_fd);
    configureKeepAliveTimes(client_fd);

    return client_fd;
}

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

static void setNonBlocking(int client_fd)
{
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "[ERROR] No se pudo configurar el socket como no bloqueante: " 
                  << strerror(errno) << std::endl;
    }
}

static void enableKeepAlive(int client_fd)
{
    int optval = 1;
    if (setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0) {
        std::cerr << "[ERROR] No se pudo habilitar keep-alive: " << strerror(errno) << std::endl;
    }
}
