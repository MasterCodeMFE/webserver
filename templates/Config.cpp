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

Config::Config( void )
{
	this->_client_max_body_size = 1 * 1024 * 1024; // 1MB by default
	this->_autoindex = false;
	this->_index = "./index.html";	
}

Config::Config( Config const &src ){(void)src;}

Config::~Config( void )
{
	ft_clearContainer(this->_v_servers);
	ft_clearContainer(this->_v_status_pages);	
}

Config	&Config::operator=( Config const &src )
{
    (void)src;
    return ( *this );
}

std::vector<Server*> const					&Config::getVServers( void )const
{
	return ( this->_v_servers );
}	

std::vector<Status *> const					&Config::getVStautsPages( void ) const
{
	return ( this->_v_status_pages );
}

unsigned int	const						&Config::getClienteMaxBodySize( void ) const
{
	return ( this->_client_max_body_size );
}

bool const									&Config::getAutoindex( void ) const
{
	return ( this->_autoindex );
}

std::string const							&Config::getIndex( void ) const
{
	return ( this->_index );
}

std::string const							&Config::getCgi( void ) const
{
	return ( this->_cgi );
}

std::string const							&Config::getRoot( void ) const
{
	return ( this->_root );
}

void										Config::addServer( Server const &server )
{
    Server *new_server = new Server(server);
	this->_v_servers.push_back(new_server);
}

void										Config::addStatusPage( int status_code, std::string page_path)
{
	Status *new_status = new Status(status_code, page_path);
	this->_v_status_pages.push_back(new_status);
}

void										Config::setClienteMaxBodySize( std::string size )
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

void										Config::setAutoindex( bool autoindex )
{
	this->_autoindex = autoindex;
}

void										Config::setIndex( std::string index )
{
	this->_index = index;
}

void										Config::setCgi( std::string cgi )
{
	this->_cgi = cgi;
}

void										Config::setRoot( std::string root )
{
	this->_root = root;	
}

