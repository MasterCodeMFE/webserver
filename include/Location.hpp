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

# include <set>
# include <string>
# include <iostream>
# include "Server.hpp"

class Location: public Server
{
	private:
		std::string									_path;
		std::set<std::string>					_s_methods;	
		std::string									_alias;

	public:
		Location( void );
		Location( Location const &src );
		Location( Server const &server, std::string path );
		~Location( void );
		
		Location	&operator=( Location const &src );

		std::string const			&getPath( void ) const;
		std::set<std::string> const	&getSMethods( void ) const;
		std::string	const			&getAlias( void ) const;

		Location 					&setPath( std::string path );
		Location 					&addSMethod( std::string method );
		Location					&setAlias( std::string alias );
};

std::ostream	&operator<<( std::ostream &o, Location const &src);

#endif
