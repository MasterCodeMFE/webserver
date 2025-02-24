/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:42:57 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 11:06:03 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include <map>
# include <vector>
# include <iostream>
# include <string>
# include <cstdlib>
# include "Status.hpp"
# include "Utils.hpp"

class Server
{
	protected:
		std::vector<std::string>			_v_server_name;
		std::vector<std::string>			_v_listen;
		std::map<int, std::string>			_m_status_pages;
        unsigned int						_client_max_body_size;
        bool								_autoindex;
		std::string							_index;
		std::string							_cgi;
		std::string							_root;
		std::map<std::string, std::string>	_m_redirections;

	public:
		Server( void );
		Server( Server const &src );
		virtual ~Server( void );

		Server	&operator=( Server const &src );
		
		virtual Server								&addServerName( std::string server_name );
		virtual Server								&addListen( std::string listen );
		Server										&addMRedirection( std::string from, std::string to);
		Server										&addStatusPage( int status_code, std::string page_path);
		Server										&setClienteMaxBodySize( std::string size );
        Server										&setAutoindex( bool autoindex );
		Server										&setIndex( std::string index );
		Server										&setCgi( std::string cgi );
		Server										&setRoot( std::string root );

		// Comprueba si hay pagina especificada, sino devuelve pagina de error por defecto.
		std::vector<std::string> const				&getServerName( void )const;
		std::vector<std::string> const				&getVListen( void )const;
		std::map<std::string, std::string> const	&getMRedirections( void ) const;
		std::map<int, std::string> const			&getMStautsPages( void ) const;
		std::string		 							getErrorPage( unsigned int status_code ) const;
		unsigned int	const						&getClienteMaxBodySize( void ) const;
		bool const									&getAutoindex( void )const;
        std::string const							&getIndex( void )const;
		std::string const							&getCgi( void )const;
		std::string const							&getRoot( void )const;

};

//std::ostream &operator<<( std::ostream &o, Server const &src );

#endif