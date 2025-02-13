/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:55:10 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/13 16:55:11 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

Server::Server( void ){}

Server::Server( Server const &src )
{
	*this = src;
}

Server::~Server( void )
{
	this->_v_server_name.clear();
	this->_v_listen.clear();
	ft_clearContainer(this->_v_locations);
	this->_m_redirections.clear();	
}

Server	&Server::operator=( Server const &src )
{
	if ( this != &src )
	{
		this->_v_server_name = src._v_server_name;
		this->_v_listen = src._v_listen;
		this->_v_locations = deepCopyContainer(src._v_locations);
		this->_m_redirections = src._m_redirections;
	}
	return( *this );
}

std::vector<std::string> const				&Server::getServerName( void )const
{
	return ( this->_v_server_name );
}

std::vector<std::string> const				&Server::getVListen( void )const
{
	return ( this->_v_listen );
}	

std::vector<Location *> const				&Server::getVLocations( void ) const
{
	return ( this->_v_locations );
}

std::map<std::string, std::string> const	&Server::getMRedirections( void ) const
{
	return ( this->_m_redirections );
}

void										Server::addServerName( std::string server_name )
{
	this->_v_server_name.push_back(server_name);
}

void										Server::addListen( std::string listen )
{
	this->_v_listen.push_back(listen);
}

void										Server::addVLocation( Location const &location )
{
    Location *new_location = new Location(location);
	this->_v_locations.push_back(new_location);
}

void										Server::addMRedirection( std::string from, std::string to)
{
	this->_m_redirections[from] = to;
}

std::ostream &operator<<( std::ostream &o, Server const &src )
{
	o << "server {" << std::endl;
	for (std::vector<std::string>::const_iterator it = src.getServerName().begin(); \
		it != src.getServerName().end(); it++)
	{
		if (it == src.getServerName().begin())
			o << "\tserver_name " ;
		o << *it;
		if (it + 1 != src.getServerName().end())
			o << " ";
		else
			o << ";" << std::endl;
	}
	
	for (std::vector<std::string>::const_iterator it = src.getVListen().begin(); \
		it != src.getVListen().end(); it++)
	{
		o << "\tlisten " << *it << ";" << std::endl;
	}

	for (std::vector<Status *>::const_iterator it = src.getVStautsPages().begin(); \
		it != src.getVStautsPages().end(); it++)
	{
		o << "\t" << **it;
	}	
	
	o << "\tclient_max_body_size " << src.getClienteMaxBodySize() << ";" << std::endl;
	
	for (std::vector<Location *>::const_iterator it = src.getVLocations().begin(); \
		it != src.getVLocations().end(); it++)
	{
		o << "\t" << **it;
	}

	for (std::map<std::string, std::string>::const_iterator it = src.getMRedirections().begin(); \
		it != src.getMRedirections().end(); it++)
	{
		o << "\tredirection " << it->first << " " << it->second << ";" << std::endl;
	}

	o << "\tautoindex " << src.getAutoindex() << ";" << std::endl;
	o << "\tindex " << src.getIndex() << ";" << std::endl;
	o << "\tcgi " << src.getCgi() << ";" << std::endl;
	o << "\troot " << src.getRoot() << ";" << std::endl;
	o << "}" << std::endl;
	return ( o );
}
