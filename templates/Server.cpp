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
Server::Server( Server const &src ){(void)src;}

Server::~Server( void )
{
	this->_v_listen.clear();
	ft_clearContainer(this->_v_status_pages);
	ft_clearContainer(this->_m_locations);
	this->_m_redirections.clear();
	
}

Server	&Server::operator=( Server const &src )
{
	(void)src;
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

std::vector<Status *> const					&Server::getVStautsPages( void ) const
{
	return ( this->_v_status_pages );
}

unsigned int	const						&Server::getClienteMaxBodySize( void ) const
{
	return ( this->_client_max_body_size );
}

std::vector<Location *> const				&Server::getMLocations( void ) const
{
	return ( this->_m_locations );
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

void										Server::setServerName( std::string server_name )
{
	this->_server_name = server_name;
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
	if (!isDigit(value)) {
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

void										Server::addMLocation( std::string path)
{
	Location *new_location = new Location(path);
	this->_m_locations.push_back(new_location);
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

