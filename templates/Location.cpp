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
	this->_client_max_body_size = 1 * 1024 * 1024; // 1MB by default
	this->_autoindex = false;
	this->_index = "./index.html";
}

Location::Location( Location const &src ): _path(src._path), \
	_v_status_pages(deepCopyContainer(src._v_status_pages)), \
	_client_max_body_size(src._client_max_body_size), \
	_v_methods(src._v_methods), _m_redirections(src._m_redirections), \
	_autoindex(src._autoindex), _index(src._index), _cgi(src._cgi), _root(src._root){}

Location::~Location( void )
{
	ft_clearContainer(this->_v_status_pages);
	this->_v_methods.clear();
	this->_m_redirections.clear();
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

std::vector<Status *> const					&Location::getVStautsPages( void ) const
{
	return ( this->_v_status_pages );
}

unsigned int	const						&Location::getClienteMaxBodySize( void ) const
{
	return ( this->_client_max_body_size );
}

std::vector<std::string> const				&Location::getVMethods( void ) const
{
	return ( this->_v_methods );
}

std::map<std::string, std::string> const	&Location::getMRedirections( void ) const
{
	return ( this->_m_redirections );
}

bool const									&Location::getAutoindex( void ) const
{
	return ( this->_autoindex );
}

std::string const							&Location::getIndex( void ) const
{
	return ( this->_index );
}

std::string const							&Location::getCgi( void ) const
{
	return ( this->_cgi );
}

std::string const							&Location::getRoot( void ) const
{
	return ( this->_root );
}

void										Location::addStatusPage( int status_code, std::string page_path)
{
	Status *new_status = new Status(status_code, page_path);
	this->_v_status_pages.push_back(new_status);
}

void										Location::setClienteMaxBodySize( std::string size )
{
	unsigned int max_body_size = 0;

	if (size.empty()) {
		this->_client_max_body_size = 1 * 1024 * 1024; // 1MB by default
		return;
	}
	std::string value = size.substr(0, size.length() - 1);
	std::string unit = size.substr(size.length() - 1);
	if (!ft_isDigit(value)) {
		std::cout << "Invalid format, client_max_body_size format only accpet"
				<< " digits followed by k, K, m, M, g, G or nothing." << std::endl;
		return;
	}
	else if (unit == "k" || unit == "K")
	{
		max_body_size = strtoul(value.c_str(), NULL, 0) * 1024;
	}
	else if (unit == "m" || unit == "M")
	{
		max_body_size = strtoul(value.c_str(), NULL, 0) * 1024 * 1024;
	}
	else if (unit == "g" || unit == "G")
	{
		max_body_size = strtoul(value.c_str(), NULL, 0) * 1024 * 1024 * 1024;
	}
	else if (unit.empty())
	{
		max_body_size = strtoul(size.c_str(), NULL, 0);
	}
	this->_client_max_body_size = max_body_size;
	
}

void										Location::addVMethod( std::string method)
{
	this->_v_methods.push_back(method);
}

void										Location::addMRedirection( std::string from, std::string to)
{
	this->_m_redirections[from] = to;
}

void										Location::setAutoindex( bool autoindex )
{
	this->_autoindex = autoindex;
}

void										Location::setIndex( std::string index )
{
	this->_index = index;
}

void										Location::setCgi( std::string cgi )
{
	this->_cgi = cgi;
}

void										Location::setRoot( std::string root )
{
	this->_root = root;	
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