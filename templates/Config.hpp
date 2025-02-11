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

#include <string>
#include <map>
#include <vector>
#include "Server.hpp"

class Config
{
	private:
		std::vector<Server*>		v_servers;
		std::vector<Status*>		m_status_pages;
		int							client_max_body;
		bool						autoindex;
		std::string					index;
		std::string					cgi;
		std::string					root;

	public:
		Config( void );
		Config( Config const &src );
		~Config( void );

		Config	&operator=( Config const &src );
};
