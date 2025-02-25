//#CABECERA

#include "Parser.hpp"

/** Excepciónes de parseo */
Parser::ParsingException::ParsingException ( std::string const &msg ): \
	std::logic_error(msg){}

/** Compueba que el número de comillas dobles `"` es  par, lo que garantiza el cierre de literales
 * entrecomillados.
 */
Parser	&Parser::_forbidenCharsCheck( void )
{
	if ( this->_cleanedConfigFile.str().find('\'') != std::string::npos \
		|| this->_cleanedConfigFile.str().find('\\') != std::string::npos \
		|| this->_cleanedConfigFile.str().find('"') != std::string::npos )
		throw Parser::ParsingException( "Forbiden char (`\"`, `'` or `\\`) detected.");
	return ( *this );
}

Parser					&Parser::_checkDirective( std::string directive)
{
	std::ostringstream oss;

	if ( Parser::_directives.end() == Parser::_directives.find( directive ))
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
		<< "Check ./src/parsing/README.md route for details.";
		throw Parser::ParsingException(oss.str());
	}
	return ( *this );
}

/** Chequea que la directiva esta acompañada de los argumentos esperados
 * y cerrada por el delimitador esperado.
 */
Parser				&Parser::_checkArgs( tokenIter kw, tokenIter end )
{
	std::ostringstream	oss;
	int					args = Parser::_directives[*kw].args;
	t_type				type = Parser::_directives[*kw].type;

	if ( std::distance( kw, end) < args + 1)
	{
		oss << "Directive " << *kw << " expected " 
			<< args << std::endl;
		throw Parser::ParsingException(oss.str()) ;
	}

	else if ( E_DIRECTIVE ==  type &&  ";" != *( kw + args + 1 ) )
	{
		oss << "Directive " << *kw << " expected " << args
			<< " arguments followed by space and `;`." << std::endl;
		throw Parser::ParsingException( oss.str() );
	}
	return ( *this );
}