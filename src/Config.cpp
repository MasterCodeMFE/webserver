/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:59:44 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 14:27:42 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config( void ){}

Config::Config( Config const &src ): ACommonConfigs( src )
{
	*this = src;
}

Config::~Config( void )
{
	ft_clearContainer(this->_v_servers);
}

Config	&Config::operator=( Config const &src )
{
    this->_v_servers = src._v_servers;
    return ( *this );
}

std::vector<Server*> const					&Config::getVServers( void )const
{
	return ( this->_v_servers );
}	

void										Config::addServer( Server const &server )
{
    Server *new_server = new Server(server);
	this->_v_servers.push_back(new_server);
}

std::ostream &operator<<( std::ostream &o, Config const &src )
{
	for (std::vector<Server*>::const_iterator it = src.getVServers().begin(); \
		it != src.getVServers().end(); ++it)
		o << **it;
	for (std::map<int, std::string>::const_iterator it = src.getMStautsPages().begin(); \
		it != src.getMStautsPages().end(); ++it)
		o << "error_code " << it->first << " " << it->second  << ";" << std::endl;
	o << "client_max_body_size " << src.getClienteMaxBodySize() << ";" << std::endl;
	o << "autoindex " << (src.getAutoindex()) << ";" << std::endl;
	o << "index " << src.getIndex() << ";" << std::endl;
	o << "cgi " << src.getCgi() << ";" << std::endl;
	o << "root " << src.getRoot() << ";" << std::endl;
	return ( o );
}
