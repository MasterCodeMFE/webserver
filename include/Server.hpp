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
		std::string					_server_name;
		std::string					_listen;
		std::map<int, std::string>	_m_status_pages;
        long unsigned int			_client_max_body_size;
        bool						_autoindex;
		std::string					_index;
		std::string					_cgi;
		std::string					_root;
		std::pair<int, std::string>	_redirection;

	public:
		Server( void );
		Server( Server const &src );
		virtual ~Server( void );

		Server	&operator=( Server const &src );

		Server										&setServerName( std::string server_name );
		Server										&setListen( std::string listen );
		Server										&setRedirection( std::string from, std::string to);
		Server										&addStatusPage( std::string const &status_string, std::string const &page_path);
		Server										&setClienteMaxBodySize( std::string size );
        Server										&setAutoindex( bool autoindex );
		Server										&setIndex( std::string index );
		Server										&setCgi( std::string cgi );
		Server										&setRoot( std::string root );

		// Comprueba si hay pagina especificada, sino devuelve pagina de error por defecto.
		std::string const							&getServerName( void )const;
		std::string const							&getListen( void )const;
		std::pair<int, std::string> const			&getRedirection( void ) const;
		std::map<int, std::string> const			&getMStautsPages( void ) const;
		std::string		 							getErrorPage( unsigned int status_code ) const;
		long unsigned int	const					&getClienteMaxBodySize( void ) const;
		bool const									&getAutoindex( void )const;
        std::string const							&getIndex( void )const;
		std::string const							&getCgi( void )const;
		std::string const							&getRoot( void )const;

};

//std::ostream &operator<<( std::ostream &o, Server const &src );

#endif