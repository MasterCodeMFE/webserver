/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserExceptions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:10:44 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 19:30:38 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

static int	isWrongPort(const std::string& port);

Parser::ParsingException::ParsingException ( std::string const &msg ): \
	std::logic_error(msg){}

Parser	&Parser::_forbidenCharsCheck( void )
{
	if ( this->_cleanedConfigFile.str().find('\'') != std::string::npos \
		|| this->_cleanedConfigFile.str().find('\\') != std::string::npos \
		|| this->_cleanedConfigFile.str().find('"') != std::string::npos )
		throw Parser::ParsingException( "Forbiden char (`\"`, `'` or `\\`) detected.");
	return ( *this );
}

Parser					&Parser::_checkDirective( std::string directive )
{
	std::ostringstream oss;

	if  ( directive.empty() )
		throw Parser::ParsingException("Sever block not closed with `}`." );
	else if ( Parser::_directives.end() == Parser::_directives.find( directive ))
	{
		oss << "Unknown directive `" << directive << "`";
		throw Parser::ParsingException(oss.str());
	}
	return ( *this );
}

Parser					&Parser::_checkContext( t_context current_context, \
	std::string directive )
{
	std::ostringstream oss;

	if ( !( current_context & Parser::_directives[directive].context ) )
	{
		oss << "Directive `" << directive
		<< "` declared out of its available scope. "
		<< "Check ./src/parsing/README.md file for details.";
		throw Parser::ParsingException(oss.str());
	}
	return ( *this );
}

Parser				&Parser::_checkArgs( tokenIter kw, tokenIter end )
{
	std::ostringstream	oss;
	int					args = Parser::_directives[*kw].args;
	t_type				type = Parser::_directives[*kw].type;

	if ( std::distance( kw, end) < args + 1)
	{
		oss << "Directive `" << *kw << "` expected " 
			<< args << " argument." << std::endl;
		throw Parser::ParsingException(oss.str()) ;
	}
	else if ( E_DIRECTIVE ==  type &&  ";" != *( kw + args + 1 ) )
	{
		oss << "Directive `" << *kw << "` expected " << args
			<< " arguments followed by space and `;`." << std::endl;
		throw Parser::ParsingException( oss.str() );
	}
	else if ( E_BLOCK ==  type && \
		( kw + args + 1 == end || "{" != *( kw + args + 1 ) ) )
	{
		oss << "Directive `" << *kw << "` expected " << args
			<< " arguments followed by space and `{`." << std::endl;
		throw Parser::ParsingException( oss.str() );
	}
	return ( *this );
}

Parser					&Parser::_checkClosedBlock( tokenIter begin, tokenIter end )
{
	std::ostringstream	oss;

	if ( end == find( begin, end, "}") )
	{
		oss << "Block directive expected `}` clossing." << std::endl;
		throw Parser::ParsingException( oss.str() );
	}
	return ( *this );
}

Parser					&Parser::_checkLocationLast( std::string directive, bool locationInServer )
{
	std::ostringstream	oss;

	if ( directive != "location" && locationInServer )
	{
		oss << "Directive `" << directive << "` in server block declared after `listen` "
			<< "directive. `listen` directives must be the lastest of `server` block." << std::endl;
		throw Parser::ParsingException( oss.str() );
	}
	return ( *this );
}

bool					Parser::_checkUniqueDirective( std::string directive, bool alreadyHasValue )
{
	std::ostringstream	oss;

	if ( alreadyHasValue == true )
	{
		oss << "Directive `" << directive << "` must only be declared one time "
			<< "per `server` block." << std::endl;
		throw Parser::ParsingException( oss.str() );
	}
	return ( !alreadyHasValue );

}


Parser					&Parser::_checkUniqueLocation( std::string path, \
	std::vector<Location> server_locations )
{
	std::ostringstream	oss;

	oss << "Duplicate location `" << path << "` in the same server." << std::endl;
	for ( std::vector<Location>::const_iterator it = server_locations.begin(); \
		it != server_locations.end(); it ++ )
	{
		if ( it->getPath() == path )
			throw Parser::ParsingException( oss.str() );
	}
	return ( *this );
}

bool		Parser::_checkHostPort( std::string hostPort )
{	
	std::ostringstream	oss;
	std::string host;
    std::string	port;
    size_t		colonPos;
	int			error;
	
	oss << hostPort << " not found for `listen` directive." << std::endl;
	colonPos = hostPort.find(':');
	error = 0;
    if (colonPos == std::string::npos)
		error = 1;
	else
	{
		host = hostPort.substr(0, colonPos);
   		port = hostPort.substr(colonPos + 1);
	}	
	if ( error || isWrongPort(port) )
		throw Parser::ParsingException( oss.str());
    return ( true );
}

static int isWrongPort(const std::string& port) 
{
    int value = std::atoi(port.c_str());
    return ( value < 0 || value > 65535 );
}