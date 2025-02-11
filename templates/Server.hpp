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
		std::string							server_name;
		std::vector<std::string>			v_listen;
		std::vector<Status*>				m_status_pages;
		int									client_max_body;
		std::vector<Location *>				m_locations;
		std::map<std::string, std::string>	m_redirections;
		bool								autoindex;
		std::string							index;
		std::string							cgi;
		std::string							root;

	public:
		Server( void );
		Server( Server const &src );
		~Server( void );

		Server	&operator=( Server const &src );
};
