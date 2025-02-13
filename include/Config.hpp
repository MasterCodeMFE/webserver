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

#ifndef CONFIG_CLASS_HPP
# define CONFIG_CLASS_HPP

# include <iostream>
# include <string>
# include <vector>
# include "ACommonConfigs.hpp"
# include "Server.hpp"

class Config: public ACommonConfigs
{
	private:
		std::vector<Server*>		_v_servers;
		
		Config( Config const &src );
		
		Config	&operator=( Config const &src );

	public:
		Config( void );
		~Config( void );

		std::vector<Server*> const					&getVServers( void )const;

		void										addServer( Server const &server );
};

std::ostream &operator<<( std::ostream &o, Config const &src );
#endif