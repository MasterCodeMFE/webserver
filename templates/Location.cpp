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

Location::Location( void )
{
	this->_client_max_body_size = 1 * 1024 * 1024; // 1MB by default
	this->_autoindex = false;
	this->_index = "./index.html";	
}
Location::Location( Location const &src ){(void)src;}

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

std::vector<Status *> const					&Location::getVStautsPages( void ) const
{
	return ( this->_v_status_pages );
}

unsigned int	const						&Location::getClienteMaxBodySize( void ) const
{
	return ( this->_client_max_body_size );
}

std::vector<std::string> const				&Location::getVMethos( void ) const
{
	return ( this->_v_methods );
}

std::map<std::string, std::string> const	&Location::getMRedirections( void ) const
{
	return ( this->_m_redirections );
}
bool const									&Location::getAutoindex( void )
{
	return ( this->_autoindex );
}
std::string const							&Location::getIndex( void )
{
	return ( this->_index );
}
std::string const							&Location::getCgi( void )
{
	return ( this->_cgi );
}
std::string const							&Location::getRoot( void )
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
	if (!_isDigit(value)) {
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

bool		Location::_isDigit( std::string size )
{
	for (std::string::const_iterator it = size.begin(); it != size.end(); ++it) {
        if (!std::isdigit(*it)) {
            return false;
        }
    }
	return ( true );
}
