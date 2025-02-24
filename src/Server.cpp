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

Server::Server( void )
{
	this->_client_max_body_size = 1 * 1024 * 1024; // 1MB by default
	this->_autoindex = false;
	this->_index = "./index.html";
	this->_root = "./"; // Establecer uno por defecto
}

Server::Server( Server const &src )
{
	*this = src;
}

Server::~Server( void )
{
	this->_v_listen.clear();
	this->_m_redirections.clear();	
}

Server	&Server::operator=( Server const &src )
{
	if ( this != &src )
	{
		this->_server_name = src._server_name;
		this->_v_listen = src._v_listen;
		this->_m_status_pages = src._m_status_pages;
		this->_client_max_body_size = src._client_max_body_size;
		this->_autoindex = src._autoindex;
		this->_index = src._index;
		this->_cgi = src._cgi;
		this->_root = src._root;
		this->_m_redirections = src._m_redirections;
	}
	return( *this );
}

std::string const							&Server::getServerName( void )const
{
	return ( this->_server_name );
}

std::vector<std::string> const				&Server::getVListen( void )const
{
	return ( this->_v_listen );
}

std::map<int, std::string> const					&Server::getMStautsPages( void ) const
{
	return ( this->_m_status_pages );
}


std::map<std::string, std::string> const	&Server::getMRedirections( void ) const
{
	return ( this->_m_redirections );
}

Server										&Server::setServerName( std::string server_name )
{
	this->_server_name = server_name;
	return( *this );
}

Server										&Server::addListen( std::string listen )
{
	this->_v_listen.push_back(listen);
	return ( *this );
}

Server										&Server::addMRedirection( std::string from, std::string to)
{
	this->_m_redirections[from] = to;
	return ( *this );
}

/** Obtiene la pagina de error asociada a un error code. */
std::string		 							Server::getErrorPage( unsigned int status_code ) const
{
	std::map<int, std::string>::const_iterator	it;

	it = this->_m_status_pages.find( status_code );
	if ( this->_m_status_pages.end() !=  it )
	{
		std::cout << status_code << "tiene error_code personalizado." << std::endl;
		return ( Status::getErrorPage( it->second ) );
	}
	return ( Status::getErrorPage( status_code ) );
}

unsigned int	const						&Server::getClienteMaxBodySize( void ) const
{
	return ( this->_client_max_body_size );
}

bool const									&Server::getAutoindex( void ) const
{
	return ( this->_autoindex );
}

std::string const							&Server::getIndex( void ) const
{
	return ( this->_index );
}

std::string const							&Server::getCgi( void ) const
{
	return ( this->_cgi );
}

std::string const							&Server::getRoot( void ) const
{
	return ( this->_root );
}

Server										&Server::setClienteMaxBodySize( std::string size )
{
	unsigned int max_body_size = 0;

	if (size.empty()) {
		this->_client_max_body_size = 1 * 1024 * 1024; // 1MB by default
		return ( *this );
	}
	std::string value = size.substr(0, size.length() - 1);
	std::string unit = size.substr(size.length() - 1);
	if (!ft_isDigit(value)) {
		std::cout << "Invalid format, client_max_body_size format only accpet"
				<< " digits followed by k, K, m, M, g, G or nothing." << std::endl;
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
	return ( *this );
	
}

Server										&Server::addStatusPage( int status_code, std::string page_path)
{
	if ( status_code < 300 || status_code > 599)
		std::cout << "Value " << status_code << " must be between 300 and 599 in error_page statement." << std::endl;
	else
		this->_m_status_pages[status_code] = page_path;
	return ( *this );
}

Server										&Server::setAutoindex( bool autoindex )
{
	this->_autoindex = autoindex;
	return ( *this );
}

Server										&Server::setIndex( std::string index )
{
	this->_index = index;
	return ( *this );
}

Server										&Server::setCgi( std::string cgi )
{
	this->_cgi = cgi;
	return ( *this );
}

Server										&Server::setRoot( std::string root )
{
	this->_root = root;
	return ( *this );
}

/*
std::ostream &operator<<( std::ostream &o, Server const &src )
{
	o << "server {" << std::endl;
	for (std::vector<std::string>::const_iterator it = src.getServerName().begin(); \
			it != src.getServerName().end(); it++)
		o << "\tserver_name " << *it << ";" << std::endl;
	
	for (std::vector<std::string>::const_iterator it = src.getVListen().begin(); \
			it != src.getVListen().end(); it++)
		o << "\tlisten " << *it << ";" << std::endl;

	for (std::map<int, std::string>::const_iterator it = src.getMStautsPages().begin(); \
			it != src.getMStautsPages().end(); ++it)
		o << "\terror_code " << it->first << " " << it->second << ";" << std::endl;
	
	o << "\tclient_max_body_size " << src.getClienteMaxBodySize() << ";" << std::endl;
	
	for (std::vector<Location *>::const_iterator it = src.getVLocations().begin(); \
			it != src.getVLocations().end(); it++)
		o << "\t" << **it;

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
*/
