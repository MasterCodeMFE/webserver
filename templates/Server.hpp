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

#include <map>
#include <vector>
#include <string>
#include "Location.hpp"
#include "Status.hpp"


class Server
{
	private:
		std::string							_server_name;
		std::vector<std::string>			_v_listen;
		std::vector<Status*>				_v_status_pages;
		int									_client_max_body_size;
		std::vector<Location *>				_m_locations;
		std::map<std::string, std::string>	_m_redirections;
		bool								_autoindex;
		std::string							_index;
		std::string							_cgi;
		std::string							_root;
		
		Server( Server const &src );

		Server	&operator=( Server const &src );

	public:
		Server( void );
		~Server( void );

		std::string const							&getServerName( void )const;
		std::vector<std::string> const				&getVListen( void )const;
		std::vector<Status *> const					&getVStautsPages( void ) const;
		unsigned int	const						&getClienteMaxBodySize( void ) const;
		std::vector<Location *> const				&getMLocations( void ) const;
		std::map<std::string, std::string> const	&getMRedirections( void ) const;
		bool const									&getAutoindex( void )const;
		std::string const							&getIndex( void )const;
		std::string const							&getCgi( void )const;
		std::string const							&getRoot( void )const;

		void										setServerName( std::string server_name );
		void										addListen( std::string listen );
		void										addStatusPage( int status_code, std::string page_path);
		void										setClienteMaxBodySize( std::string size );
		void										addMLocation( std::string path);
		void										addMRedirection( std::string from, std::string to);
		void										setAutoindex( bool autoindex );
		void										setIndex( std::string index );
		void										setCgi( std::string cgi );
		void										setRoot( std::string root );
};