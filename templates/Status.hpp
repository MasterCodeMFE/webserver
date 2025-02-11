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
# include <fstream>
# include <sstream>

class Status
{
	private:	
		static std::map<int, std::string>	_m_status_responses;
		int									_status_code;
		std::string							_error_page_path;

		Status( void );

		Status	&operator=( Status const &src );
		
		static void							_setStatusResponses( void );
		
	public:
		Status( Status const &src );
		Status( int status_code, std::string error_page_path );
		~Status( void );


		static std::string					getDefaultStatusResponse( int status_code );
		static std::string 					getDefaultErrorPage( int status_code );
		
		int									getStatusCode( void ) const;
		std::string const					&getErrorPagePath( void ) const;
		std::string	 						getErrorPage( void ) const;
};

std::ostream	&operator<<( std::ostream &o, Status &src);

#endif
