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
# include "ACommonConfigs.hpp"
# include "AServerBlockConfigs.hpp"

class Location: public ACommonConfigs, public AServerBlockConfigs
{
	private:
		std::string									_path;
		std::vector<std::string>					_v_methods;	

		Location( void );

		Location	&operator=( Location const &src );

	public:
		Location( Location const &src );
		Location( std::string path );
		~Location( void );

		std::string const							&getPath( void ) const;
		std::vector<std::string> const				&getVMethods( void ) const;

		void										addVMethod( std::string method );
};

std::ostream	&operator<<( std::ostream &o, Location const &src);

#endif
