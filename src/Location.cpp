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


Location::Location( std::string path )
{
	this->_path = path;
}

Location::Location( Location const &src ): ACommonConfigs( src ), \
	AServerBlockConfigs( src ), _path(src._path), _v_methods(src._v_methods)
{
	this->_v_status_pages = deepCopyContainer(src._v_status_pages);
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

void										Location::addVMethod( std::string method)
{
	this->_v_methods.push_back(method);
}

std::ostream	&operator<<( std::ostream &o, Location const &src)
{
	o << "location " << src.getPath() << " {" << std::endl;
	for (std::vector<Status *>::const_iterator it = src.getVStautsPages().begin(); \
		it != src.getVStautsPages().end(); it++)
		o << "\t\t" << **it;
	o << "\t\tclient_max_body_size " << src.getClienteMaxBodySize() << ";" << std::endl;
	for (std::vector<std::string>::const_iterator it = src.getVMethods().begin(); \
		it != src.getVMethods().end(); it++)
	{
		if (it == src.getVMethods().begin())
			o << "\t\tmethods ";
		o << *it;
		if (it + 1 != src.getVMethods().end())
			o << " ";
		else
			o << ";" << std::endl;
	}
	for (std::map<std::string, std::string>::const_iterator it = src.getMRedirections().begin(); \
		it != src.getMRedirections().end(); it++)
		o << "\t\tredirection " << it->first << " " << it->second << ";"  << std::endl;
	o << "\t\tautoindex " << src.getAutoindex() << ";" << std::endl;
	o << "\t\tindex " << src.getIndex() << ";" << std::endl;
	o << "\t\tcgi " << src.getCgi() << ";" << std::endl;
	o << "\t\troot " << src.getRoot() << ";" << std::endl;
	o << "\t}" << std::endl;
	return ( o );
}