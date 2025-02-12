/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:40:25 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 14:27:45 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_CLASS_HPP
# define CONFIG_CLASS_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include "Server.hpp"
# include "Status.hpp"
# include "Utils.hpp"



class Config
{
	private:
		std::vector<Server*>		_v_servers;
		std::vector<Status*>		_v_status_pages;
		unsigned int				_client_max_body_size;
		bool						_autoindex;
		std::string					_index;
		std::string					_cgi;
		std::string					_root;
		
		Config( Config const &src );
		
		Config	&operator=( Config const &src );

	public:
		Config( void );
		~Config( void );

		std::vector<Server*> const					&getVServers( void )const;
		std::vector<Status *> const					&getVStautsPages( void ) const;
		unsigned int	const						&getClienteMaxBodySize( void ) const;
		bool const									&getAutoindex( void )const;
		std::string const							&getIndex( void )const;
		std::string const							&getCgi( void )const;
		std::string const							&getRoot( void )const;

		void										addServer( Server const &server );
		void										addStatusPage( int status_code, std::string page_path);
		void										setClienteMaxBodySize( std::string size );
		void										setAutoindex( bool autoindex );
		void										setIndex( std::string index );
		void										setCgi( std::string cgi );
		void										setRoot( std::string root );
};

std::ostream &operator<<( std::ostream &o, Config const &src );
#endif