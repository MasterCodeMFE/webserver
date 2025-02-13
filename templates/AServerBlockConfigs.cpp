/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerBlockConfigs.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:53:55 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/13 16:53:57 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "AServerBlockConfigs.hpp"

AServerBlockConfigs::AServerBlockConfigs( void ){}

AServerBlockConfigs::AServerBlockConfigs( AServerBlockConfigs const &src )
{
	*this = src;
}

AServerBlockConfigs::~AServerBlockConfigs( void )
{
	this->_m_redirections.clear();	
}

AServerBlockConfigs	&AServerBlockConfigs::operator=( AServerBlockConfigs const &src )
{
	if ( this != &src )
	{
		this->_m_redirections = src._m_redirections;
	}
	return( *this );
}

std::map<std::string, std::string> const	&AServerBlockConfigs::getMRedirections( void ) const
{
	return ( this->_m_redirections );
}

void										AServerBlockConfigs::addMRedirection( std::string from, std::string to)
{
	this->_m_redirections[from] = to;
}
