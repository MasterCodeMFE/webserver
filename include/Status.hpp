/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 11:06:18 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/03/11 11:55:36 by manufern         ###   ########.fr       */
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

		Status( void );
		Status( Status const &src );
		~Status( void );

		Status	&operator=( Status const &src );
		
		static void							_setStatusResponses( void );
		static bool							_inErrorRange( int status_code );
		
	public:
		static std::string					getStatusResponse( int status_code );
		static std::string 					getErrorPage( int status_code );
		static std::string 					getErrorPage( std::string code_file_path, int status_code );
};

#endif
