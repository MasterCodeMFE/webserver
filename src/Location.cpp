/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:54:05 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/03/31 19:24:40 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Location.hpp"

Location::Location( void ){}

Location::Location( Server const &server, std::string path ): Server( server ), \
	_path(path){}

Location::Location( Location const &src ): Server( src ), \
	_path(src._path), _s_methods(src._s_methods), _alias(src._alias){}

Location::~Location( void )
{
	this->_s_methods.clear();
}

Location	&Location::operator=( Location const &src )
{
	if (this != &src) {
		Server::operator=(src);
		this->_path = src._path;
		this->_s_methods = src._s_methods;
		this->_alias = src._alias;
	}
	return( *this );
}

std::string const							&Location::getPath( void ) const
{
	return ( this->_path );
}

std::set<std::string> const				&Location::getSMethods( void ) const
{
	return ( this->_s_methods );
}

std::string const				&Location::getAlias( void ) const
{
	return ( this->_alias);
}

Location									&Location::setPath( std::string path )
{
	this->_path = path;
	return ( *this );
}

Location									&Location::addSMethod( std::string method)
{
	this->_s_methods.insert(method);
	return ( *this );
}

Location									&Location::setAlias( std::string alias )
{
	this->_alias = this->_root.substr( 0, this->_root.find("/", 2)) + alias;
	return ( *this );
}

std::ostream	&operator<<( std::ostream &o, Location const &src)
{
	o << "location " << src.getPath() << " {" << std::endl;
	o << "\tserver_name " << src.getServerName() << ";" << std::endl;
	o << "\tlisten " << src.getListen() << ";" << std::endl;

	for (std::map<int, std::string>::const_iterator it = src.getMStautsPages().begin(); \
			it != src.getMStautsPages().end(); ++it)
		o << "\terror_page " << it->first << " " << it->second << ";" << std::endl;
	
	o << "\tclient_max_body_size " << src.getClienteMaxBodySize() << ";" << std::endl;

	o << "\tautoindex " << src.getAutoindex() << ";" << std::endl;
	o << "\tindex " << src.getIndex() << ";" << std::endl;
	o << "\tcgi " << src.getCgi() << ";" << std::endl;
	o << "\troot " << src.getRoot() << ";" << std::endl;
	o << "\talias " << src.getAlias() << ";" << std::endl;
	o << "\tredirection " << src.getRedirection().first << " " << src.getRedirection().second << ";" << std::endl;
	for (std::set<std::string>::const_iterator it = src.getSMethods().begin(); \
		it != src.getSMethods().end(); it++)
		o << "\tmethod " << *it << ";" << std::endl;
	o << "\t}" << std::endl;
	return ( o );
}