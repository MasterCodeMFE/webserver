/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:54:05 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 09:36:55 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Location.hpp"

Location::Location( void ){}


Location::Location( Server const &server, std::string path ): Server( server ), \
	_path(path){}

Location::Location( Location const &src ): Server( src ), \
	_path(src._path), _v_methods(src._v_methods), _alias(src._alias)
{
	this->_m_status_pages = src._m_status_pages;
	this->_client_max_body_size = src._client_max_body_size;
	this->_m_redirections = src._m_redirections;
	this->_autoindex = src._autoindex;
	this->_index = src._index;
	this->_cgi = src._cgi;
	this->_root = src._root;
}

Location::~Location( void )
{
	this->_v_methods.clear();
}

Location	&Location::operator=( Location const &src )
{
	(void)src;
	return( *this );
}

std::string const							&Location::getPath( void ) const
{
	return ( this->_path );
}

std::vector<std::string> const				&Location::getVMethods( void ) const
{
	return ( this->_v_methods );
}

Location									&Location::addVMethod( std::string method)
{
	this->_v_methods.push_back(method);
	return ( *this );
}

Location									&Location::setAlias( std::string alias )
{
	this->_alias = alias;
	return ( *this );
}

Location									&Location::addServerName( std::string server_name )
{
	( void )server_name;
	std::cout << "Location block doesn't accept server_name directive." << std::endl;
	return ( *this );
}


Location									&Location::addListen( std::string listen )
{
	( void )listen;
	std::cout << "Location block doesn't accept listen directive." << std::endl;
	return ( *this );
}



std::ostream	&operator<<( std::ostream &o, Location const &src)
{
	o << "location " << src.getPath() << " {" << std::endl;
	o << "\tserver_name " << src.getServerName() << ";" << std::endl;
	
	for (std::vector<std::string>::const_iterator it = src.getVListen().begin(); \
			it != src.getVListen().end(); it++)
		o << "\tlisten " << *it << ";" << std::endl;

	for (std::map<int, std::string>::const_iterator it = src.getMStautsPages().begin(); \
			it != src.getMStautsPages().end(); ++it)
		o << "\terror_code " << it->first << " " << it->second << ";" << std::endl;
	
	o << "\tclient_max_body_size " << src.getClienteMaxBodySize() << ";" << std::endl;

	o << "\t\tautoindex " << src.getAutoindex() << ";" << std::endl;
	o << "\t\tindex " << src.getIndex() << ";" << std::endl;
	o << "\t\tcgi " << src.getCgi() << ";" << std::endl;
	o << "\t\troot " << src.getRoot() << ";" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = src.getMRedirections().begin(); \
		it != src.getMRedirections().end(); it++)
		o << "\t\tredirection " << it->first << " " << it->second << ";"  << std::endl;
	for (std::vector<std::string>::const_iterator it = src.getVMethods().begin(); \
		it != src.getVMethods().end(); it++)
		o << "\t\tmethod " << *it << ";" << std::endl;
	o << "\t}" << std::endl;
	return ( o );
}