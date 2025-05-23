/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeployServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:13:35 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 19:23:17 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEPLOY_SERVER_HPP
# define DEPLOY_SERVER_HPP

# include <vector>
# include <poll.h>

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <poll.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <filesystem>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <sys/stat.h>
#include <dirent.h>
#include <netdb.h>
#include <signal.h>
#include <iterator>
#include <ctime> 

# include "Location.hpp"

extern volatile sig_atomic_t g_signal;

typedef std::set<std::pair<std::string, std::string> > listenSet;


struct HttpRequest {
	std::string method;
	std::string path;
	std::string protocol;
	std::map<std::string, std::string> headers;
	std::string body;          
	std::string query_string;  
};


class DeployServer
{
	private:
		listenSet				listeners; 
		std::vector<int>		server_fds; 
		std::vector<Location>	locations; 
		std::vector<pollfd>     fds;
		
		DeployServer( void );
		DeployServer( DeployServer const &src );

		DeployServer    &operator=( DeployServer const &src );
		
		DeployServer	&_init_server_sockets( void );
		int				_run_server_event_loop( void );
		int				_setup_server_listeners(void );
		void    		_process_server_events( void );
		void    		_process_client_events( size_t start_index );
		int				_accept_client_connection(int server_fd );
		int 			_handle_client_request( int client_fd );
		int				_dispatch_http_request(int client_fd, \
							HttpRequest& httpRequest );
		std::string 	_handle_redirection(int status_code, const std::string& location, const Location& location_config);					

	public:
		DeployServer( std::vector<Location> locations );
		~DeployServer( void );

		int initialize_server_sockets( void );
};

int configure_and_bind_socket(int server_fd, std::pair<std::string, std::string> const &config);
Location findLocation(const HttpRequest& httpRequest, std::vector<Location> locations);


#endif