/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPolling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:08:05 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 20:09:32 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "DeployServer.hpp"


int DeployServer::_run_server_event_loop( void )
{

    if (this->_setup_server_listeners() == -1)
        return -1;

    while (!g_signal )
    {

        int ret = poll(this->fds.data(), this->fds.size(), -1);
        if (ret == -1 && !g_signal)
        {
            std::cerr << "[ERROR] Error en poll(): " << strerror(errno) << std::endl;
            return -1;
        }
        this->_process_server_events( );

        this->_process_client_events(server_fds.size() );
    }
    return 0;
}

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
        pollfd server_pollfd = {};
        server_pollfd.fd = this->server_fds[i];
        server_pollfd.events = POLLIN | POLLOUT;
        this->fds.push_back(server_pollfd);
        listenit ++;
    }
    return 0;
}

void    DeployServer::_process_server_events( void )
{
    for (size_t i = 0; i < this->server_fds.size(); i++)
    {
        if (this->fds[i].revents & POLLIN)
        {
            int new_client_fd = this->_accept_client_connection(this->server_fds[i]);
            if (new_client_fd != -1)
            {
                pollfd new_pollfd = {};
                new_pollfd.fd = new_client_fd;
                new_pollfd.events = POLLIN | POLLOUT;
                this->fds.push_back(new_pollfd);
            }
        }
    }
}

void DeployServer::_process_client_events(size_t start_index)
{
    for (size_t i = start_index; i < this->fds.size(); ++i)
    {
        if (this->fds[i].revents & (POLLIN | POLLHUP | POLLERR))
        {
            if (this->fds[i].revents & POLLIN)
            {
                int res = this->_handle_client_request(this->fds[i].fd);
                if (res == -1)
                {
                    std::cout << "[INFO] Cliente desconectado: " << this->fds[i].fd << "\n";
                    close(this->fds[i].fd);
                    this->fds.erase(this->fds.begin() + i);
                    --i;
                }
            }
        }
    }
}


