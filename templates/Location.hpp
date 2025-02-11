/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:54:05 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 09:36:55 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <map>
# include <vector>
# include <string>
# include "Status.hpp"

class Location
{
	private:
		std::vector<Status*>				m_status_pages;
		int									client_max_body;
		std::vector<std::string>			v_methods;	
		std::map<std::string, std::string>	m_redirections;
		bool								autoindex;
		std::string							index;
		std::string							cgi;
		std::string							root;
	protected:

	public:
		Location( void );
		Location( Location const &src );
		~Location( void );

		Location	&operator=( Location const &src );
}
