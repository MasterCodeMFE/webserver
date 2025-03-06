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

/** Comprueba si la directiva recibida como argumento forma parte de las directivas 
 * reconocidas en el fichero de configuración.
 * @param directive String de la directiva a comprobar.
 * 
 * @return Auto referencia al objeto Parser para poder encadenar metodos de la clase.
 */
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

/** Comprueba si en el `current_context` está autorizada la directiva `directive`
 * @param current_context Valor de la enumeración `t_context` que especifica el bloque 
 * 		en el que se encuentra actualmente el parser.
 * @param directive String de la directiva a comprobar.
 * 
 * @return Auto referencia al objeto Parser para poder encadenar metodos de la clase.
 */
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

/** Chequea que la directiva esté acompañada de los argumentos esperados
 * y cerrada por el delimitador esperado.
 * @param kw Iterador que apunta al identificador de la directive.
 * @param end Iterador que apunta al final de la cadena de tokens a analizar.
 * 
 * @return Auto referencia al objeto Parser para poder encadenar metodos de la clase.
 */
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

/** Comprueba que las directivas de tipo bloque estén cerradas con `}`.
 * @param begin Iterador que apunta al identificador de la directiva.
 * @param end Iterador al final del vector de tokens a analizar. 
 * 
 * @return Auto referencia al objeto Parser para poder encadenar metodos de la clase.
 */
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

/** Comprueba si una directiva en un bloque `server` está declarada  tras la directiva `location`,
 * la cual tiene que ser obligatoriamente la última en declararse dentro del bloque.
 * 
 * @param directive Identificados de la directiva.
 * @param locationInSever Booleano que identifica si ya se a procesado alguna directiva `location`
 * 	dentro del bloque `server`
 * 
 * @return Auto referencia al objeto Parser para poder encadenar metodos de la clase.
 */
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

/** Comprueba las directivas que tienen que tener una única declaración por bloque `server`.
 * @param directive Directiva que tiene que estar declarada una vez o ninguna por `server`
 * @param alreadyHasValue Booleano que indica si dicha directiva ya ha sido declarada en el
 * 			bloque `server`.
 * 
 * @return Resultado de la comprobación 
 */
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


/** Comprueba que las rutas declaradas en un servidor son únicas`.
 * @param path Ruta de la nueva `location`.
 * @param server_lcoations localizaciones actuales en el bloque `server`.
 * 
 * @return Resultado de la comprobación 
 */
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

