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
# include <string>
# include "ACommonConfigs.hpp"
# include "AServerBlockConfigs.hpp"
# include "Location.hpp"

class Location;

class Server: public ACommonConfigs, public AServerBlockConfigs
{
	private:
		std::vector<std::string>			_v_server_name;
		std::vector<std::string>			_v_listen;
		std::vector<Location *>				_v_locations;
		std::map<std::string, std::string>	_m_redirections;

	public:
		Server( void );
		Server( Server const &src );
		~Server( void );

		Server	&operator=( Server const &src );

		std::vector<std::string> const				&getServerName( void )const;
		std::vector<std::string> const				&getVListen( void )const;
		std::vector<Location *> const				&getVLocations( void ) const;
		std::map<std::string, std::string> const	&getMRedirections( void ) const;

		void										addServerName( std::string server_name );
		void										addListen( std::string listen );
		void										addVLocation( Location const &location );
		void										addMRedirection( std::string from, std::string to);
};

std::ostream &operator<<( std::ostream &o, Server const &src );

#endif