/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:54:05 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 09:36:55 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

Server::Server( void )
{
	this->_client_max_body_size = 1 * 1024 * 1024; // 1MB by default
	this->_autoindex = false;
	this->_index = "./index.html";	
}

Server::Server( Server const &src )
{
	*this = src;
}

Server::~Server( void )
{
	this->_v_listen.clear();
	ft_clearContainer(this->_v_status_pages);
	ft_clearContainer(this->_v_locations);
	this->_m_redirections.clear();	
}

Server	&Server::operator=( Server const &src )
{
	if ( this != &src )
	{
		this->_server_name = src._server_name;
		this->_v_listen = src._v_listen;
		this->_v_status_pages = deepCopyContainer(src._v_status_pages);
		this->_client_max_body_size = src._client_max_body_size;
		this->_v_locations = deepCopyContainer(src._v_locations);
		this->_m_redirections = src._m_redirections;
		this->_autoindex = src._autoindex;
		this->_index = src._index;
		this->_cgi = src._cgi;
		this->_root = src._root;
	}
	return( *this );
}

std::vector<std::string> const				&Server::getServerName( void )const
{
	return ( this->_server_name );
}

std::vector<std::string> const				&Server::getVListen( void )const
{
	return ( this->_v_listen );
}	

std::vector<Status *> const					&Server::getVStautsPages( void ) const
{
	return ( this->_v_status_pages );
}

unsigned int	const						&Server::getClienteMaxBodySize( void ) const
{
	return ( this->_client_max_body_size );
}

std::vector<Location *> const				&Server::getVLocations( void ) const
{
	return ( this->_v_locations );
}

std::map<std::string, std::string> const	&Server::getMRedirections( void ) const
{
	return ( this->_m_redirections );
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

void										Server::addServerName( std::string server_name )
{
	this->_server_name.push_back(server_name);
}

void										Server::addListen( std::string listen )
{
	this->_v_listen.push_back(listen);
}

void										Server::addStatusPage( int status_code, std::string page_path)
{
	Status *new_status = new Status(status_code, page_path);
	this->_v_status_pages.push_back(new_status);
}

void										Server::setClienteMaxBodySize( std::string size )
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

void										Server::addVLocation( Location const &location )
{
    Location *new_location = new Location(location);
	this->_v_locations.push_back(new_location);
}

void										Server::addMRedirection( std::string from, std::string to)
{
	this->_m_redirections[from] = to;
}

void										Server::setAutoindex( bool autoindex )
{
	this->_autoindex = autoindex;
}

void										Server::setIndex( std::string index )
{
	this->_index = index;
}

void										Server::setCgi( std::string cgi )
{
	this->_cgi = cgi;
}

void										Server::setRoot( std::string root )
{
	this->_root = root;	
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