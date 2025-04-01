/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:08:15 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 20:09:35 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Parser.hpp"

std::map<std::string, t_directive>	Parser::_directives;

void	Parser::_setDirectives( void )
{
	_directives["server"] = build_directive( 0, E_GLOBAL, E_BLOCK );
	_directives["server_name"] = build_directive( 1, E_SERVER, E_DIRECTIVE );
	_directives["listen"] = build_directive( 1, E_SERVER, E_DIRECTIVE );
	_directives["error_page"] = build_directive( 2, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["client_max_body_size"] = build_directive( 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["location"] = build_directive( 1, E_SERVER, E_BLOCK );
	_directives["method"] = build_directive( 1, E_LOCATION, E_DIRECTIVE );
	_directives["return"] = build_directive( 2, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["autoindex"] = build_directive( 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["index"] = build_directive( 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["cgi"] = build_directive( 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["root"] = build_directive( 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["alias"] = build_directive( 1, E_LOCATION, E_DIRECTIVE );
}
Parser::Parser( void ): _configFile( "./webserv.conf" ){}

Parser::Parser( Parser const &src){ (void)src; }

Parser::~Parser( void ){}

Parser	&Parser::operator=( Parser const &src ) { ( void )src; return ( *this ); }

Parser	&Parser::_cleanComments( std::string str )
{
	size_t	haystack = str.find( '#' );

	if ( haystack != std::string::npos )
		str = str.substr( 0, haystack);
	this->_cleanedConfigFile << str;
	if ( !str.empty() )
		this->_cleanedConfigFile << '\n';
	return( *this );
}
Parser	&Parser::_tokenizeConfig( void )
{
	char		*cleaned_str = new char[strlen(this->_cleanedConfigFile.str().c_str()) + 1];
	const char	delimiters[] = " \t\f\v\n\r";
	char		*token;
	
	strcpy(cleaned_str, this->_cleanedConfigFile.str().c_str());
	token = strtok(cleaned_str, delimiters);
	while ( token != NULL )
	{
		_tokens.push_back( token );
		token = strtok(NULL, delimiters);
	}

	delete[] cleaned_str;
	return ( *this );
}

Parser	&Parser::setConfigFile( const char* config_file_path )
{
	this->_configFile.close();
	this->_configFile.open (config_file_path);
	return ( *this );
}

void 	Parser::parseConfigFile( std::vector<Location> &loc )
{
	std::string str;
	if ( !this->_configFile.is_open() )
		throw Parser::ParsingException( "Failure on file opening");
	while ( std::getline(this->_configFile, str) )
		this->_cleanComments(str);
	this->_configFile.close();
	this->_forbidenCharsCheck()._tokenizeConfig()._processTokens( loc );
}

void		Parser::_processTokens( std::vector<Location> &loc )
{
	tokenIter it = this->_tokens.begin();
	if ( Parser::_directives.empty())
		Parser::_setDirectives();
    while ( it != this->_tokens.end())
	{
		this->_checkDirective( *it )._checkContext( E_GLOBAL, *it);
		
		if ( "server" == *it )
		{
			std::vector<std::string> subvector(it, this->_tokens.end());
			it += this->_serverProcessing( loc, subvector );
		}
		if ( it != this->_tokens.end() )
			it ++;
	}
}

t_directive	build_directive( int args, unsigned int context, t_type type )
{
	t_directive	dir;

	dir.args = args;
	dir.context = context;
	dir.type = type;
	return ( dir );
}

int	Parser::_serverProcessing( std::vector<Location> &locs, \
			std::vector<std::string> v_str )
{
	tokenIter									it = v_str.begin();
	std::vector<Location> 						server_locations;
	Location									server;	

	if ( v_str.end() == ++it || *it != "{" )
		throw Parser::ParsingException("Expected space followed by `{` after `server` directive" );	
	
	if ( v_str.end() == ++it )
		this->_checkDirective( "" );
	if ( *it == "}" )
	{
		std::cout << "\033[1;38;5;226m" << "[WARNING] Empty server ignored." << "\033[0m" << std::endl;
		return ( std::distance( v_str.begin(), it) );
	}
	while ( it != v_str.end() && *it != "}" )
	{
		this->_checkDirective( *it )
			._checkContext( E_SERVER, *it )
			._checkArgs( it, v_str.end())
			._checkLocationLast( *it, !server_locations.empty() );

		if ( *it == "location" )
		{
			Location	new_location(server);
			tokenIter	block_close = find( it, v_str.end(), "}");

			new_location.setPath( *(++it) );
			++it;
			this->_checkClosedBlock( it++, v_str.end())
				._checkUniqueLocation( new_location.getPath(), server_locations )
				._handleLocationDirective(new_location, it, block_close );
			server_locations.push_back(new_location);
			it = block_close;
			this->_checkClosedBlock( it + 1, v_str.end());
		}
		else
		{
			this->_handleServerDirective( server, it);
		}
		it ++;
	}
	if ( !server_locations.empty() )
		locs.insert( locs.end(), server_locations.begin(), server_locations.end());
	else
		locs.push_back( server );
	return ( std::distance( v_str.begin(), it) );
}
void					Parser::_handleServerDirective(	Location &server, tokenIter &it )
{
	if ( "server_name" == *it \
		&& this->_checkUniqueDirective( *it, !server.getServerName().empty()) )
		server.setServerName( *(++it) );
	else if ( "listen" == *it \
		&& this->_checkUniqueDirective( *it, !server.getListen().empty()) 
		&& this->_checkHostPort( *(it + 1 )))
		server.setListen( *(++it) );
	else if ( "error_page" == *it )
	{
		server.addStatusPage( *(it + 1), *(it + 2) );
		it += 2;
	}
	else if ( "client_max_body_size" == *it )
		server.setClienteMaxBodySize( *(++it) );
	else if ( "method" == *it )
		server.addSMethod( *(++it) );
	else if ( "return" == *it )
	{
		server.setRedirection( *(it + 1), *(it + 2) );
		it += 2;
	}
	else if ( "autoindex" == *it )
		server.setAutoindex( *(++it) == "on" );
	else if ( "index" == *it )
		server.setIndex( *(++it) );
	else if ( "cgi" == *it )
		server.setCgi( *(++it) );
	else if ( "root" == *it )
		server.setRoot( *(++it) );
	else if ( "alias" == *it )
		server.setAlias( *(++it) );
	it++;	
}

void	Parser::_handleLocationDirective( Location &location, tokenIter &it, tokenIter end )
{
	if ( it == end )
		location.setPath( std::string() );
	while ( it != end )
	{
		this->_checkDirective( *it )
			._checkContext( E_LOCATION, *it )
			._checkArgs( it, end);
		this->_handleServerDirective( location, it);
		it++;
	}			
}
