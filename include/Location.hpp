/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:54:05 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/13 16:55:02 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CLASS_HPP
# define LOCATION_CLASS_HPP

# include <vector>
# include <string>
# include <iostream>
# include "Server.hpp"

class Location: public Server
{
	private:
		std::string									_path;
		std::vector<std::string>					_v_methods;	
		std::string									_alias;

		Location( void );

		Location	&operator=( Location const &src );

	public:
		Location( Location const &src );
		Location( Server const &server, std::string path );
		~Location( void );

		std::string const							&getPath( void ) const;
		std::vector<std::string> const				&getVMethods( void ) const;

		Location 									&addVMethod( std::string method );
		Location									&setAlias( std::string alias );
		Location									&addServerName( std::string server_name );
		Location									&addListen( std::string listen );

};

std::ostream	&operator<<( std::ostream &o, Location const &src);

#endif
