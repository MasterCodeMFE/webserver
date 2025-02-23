/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommonConfigs.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:59:44 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 14:27:42 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommonConfigs.hpp"

ACommonConfigs::ACommonConfigs( void )
{
	this->_client_max_body_size = 1 * 1024 * 1024; // 1MB by default
	this->_autoindex = false;
	this->_index = "./index.html";
	this->_root = "./"; // Establecer uno por defecto
}

ACommonConfigs::ACommonConfigs( ACommonConfigs const &src )
{
	*this = src;
}

ACommonConfigs::~ACommonConfigs( void )
{
	this->_m_status_pages.clear();	
}

ACommonConfigs	&ACommonConfigs::operator=( ACommonConfigs const &src )
{
	if ( this != &src )
	{
		this->_m_status_pages = src._m_status_pages;
		this->_client_max_body_size = src._client_max_body_size;
		this->_autoindex = src._autoindex;
		this->_index = src._index;
		this->_cgi = src._cgi;
		this->_root = src._root;
	}
    return ( *this );
}

std::map<int, std::string> const					&ACommonConfigs::getMStautsPages( void ) const
{
	return ( this->_m_status_pages );
}

unsigned int	const						&ACommonConfigs::getClienteMaxBodySize( void ) const
{
	return ( this->_client_max_body_size );
}

bool const									&ACommonConfigs::getAutoindex( void ) const
{
	return ( this->_autoindex );
}

std::string const							&ACommonConfigs::getIndex( void ) const
{
	return ( this->_index );
}

std::string const							&ACommonConfigs::getCgi( void ) const
{
	return ( this->_cgi );
}

std::string const							&ACommonConfigs::getRoot( void ) const
{
	return ( this->_root );
}

void										ACommonConfigs::addStatusPage( int status_code, std::string page_path)
{
	if ( status_code < 300 || status_code > 599)
		std::cout << "Value " << status_code << " must be between 300 and 599 in error_page statement." << std::endl;
	else
		this->_m_status_pages[status_code] = page_path;
}

void										ACommonConfigs::setClienteMaxBodySize( std::string size )
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

void										ACommonConfigs::setAutoindex( bool autoindex )
{
	this->_autoindex = autoindex;
}

void										ACommonConfigs::setIndex( std::string index )
{
	this->_index = index;
}

void										ACommonConfigs::setCgi( std::string cgi )
{
	this->_cgi = cgi;
}

void										ACommonConfigs::setRoot( std::string root )
{
	this->_root = root;	
}
