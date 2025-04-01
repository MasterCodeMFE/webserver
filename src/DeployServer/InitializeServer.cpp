/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitializeServer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:52 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 19:58:51 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeployServer.hpp"

static int setup_server_socket( std::pair<std::string, std::string> const &listener );
static int create_and_configure_socket( void );
static void close_socket(int server_fd);

int DeployServer::initialize_server_sockets( void ) {
	this->_init_server_sockets( );
	if (server_fds.empty()) {
		std::cerr << "No se pudieron inicializar los sockets del servidor." << std::endl;
		return 1;
	}
	if (this->_run_server_event_loop() == -1) {
		for (size_t i = 0; i < server_fds.size(); i++) {
			close_socket(server_fds[i]);
		}
		return 1;
	}
	for (size_t i = 0; i < server_fds.size(); i++) {
		close_socket(server_fds[i]);
	}
	return 0; 
}

DeployServer	&DeployServer::_init_server_sockets(void) {
	int server_fd;
	for (listenSet::iterator it = this->listeners.begin(); it != this->listeners.end();  ) {
		server_fd = setup_server_socket(*it);
		if (server_fd != -1) { 
		{
			this->server_fds.push_back(server_fd);
			it++;
		}
		} else {
			std::cerr << "No se pudo configurar el socket para el servidor `" << it->second << "` (" << it->first << ")" << std::endl;

			listenSet::iterator tmp = it;
			tmp ++;
			this->listeners.erase(it);
			it = tmp;                       
		}
	}

	return *this;
}


static int setup_server_socket( std::pair<std::string, std::string> const &listener )
{
	int server_fd = create_and_configure_socket();
	if (server_fd == -1) { 
		std::cerr << "Error creando socket para servidor `" << listener.second 
			<< "`(" << listener.first << ")"<<  std::endl;
		return -1;
	}
	if ( configure_and_bind_socket(server_fd, listener) == -1)
	{
		close_socket(server_fd);
		return -1;
	}
	return server_fd;
}

static int create_and_configure_socket()
{

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) { 
		std::cerr << "Error al crear el socket." << std::endl;
		return -1;
	}

	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << "Error al configurar SO_REUSEADDR." << std::endl;
		close_socket(server_fd); 
		return -1;
	}
	return server_fd; 
}



static void close_socket(int server_fd)
{
	close(server_fd);
}

