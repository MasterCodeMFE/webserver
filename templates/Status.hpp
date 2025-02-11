/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 11:06:18 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 15:35:09 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_CLASS_HPP
# define STATUS_CLASS_HPP
# include <map>
# include <string>
# include <iostream>
# include <sstream>

class Status
{
	private:	
		static std::map<int, std::string>	_m_status_responses;
		int									_status_code;
		std::string							_error_page_path;

		Status( void );
		Status( Status const &src );

		Status	&operator=( Status const &src );
		
	public:
		Status( int status_code, std::string error_page_path );
		~Status( void );

		static void							setStatusResponses( void );

		static std::string const 			&getStatusResponse( int status_code );
		static std::string const			&getDefaultErrorPage( int status_code );
		
		int									getStatusCode( void ) const;
		std::string const					&getErrorPagePath( void ) const;
		std::string	const					&getErrorPage( int status_code ) const;
};

std::ostream	&operator<<( std::ostream &o, Status &src);

#endif
