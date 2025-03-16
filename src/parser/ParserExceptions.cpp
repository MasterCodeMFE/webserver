/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserExceptions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:10:44 by manufern          #+#    #+#             */
/*   Updated: 2025/03/06 19:10:48 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

static int	isWrongHost(const std::string& ip);
static int	isWrongPort(const std::string& port);

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

/** Comprueba que la ip y puerto de `listen`esten en el formato y rango correcto.
 * @param hostPort Contiene un string con la IP y puerto a escuhar. Para 
 * 		ser correcto debe tener el formato [0-255].[0-255].[0-255].[0-255]:[0-65535].
 * @return `true` si el formato es correcto. Lanza un excepción sin no se cumplen las condiciones. 
 */
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
	if ( error || isWrongHost(host) || isWrongPort(port) )
		throw Parser::ParsingException( oss.str());
    return ( true );
}

/** Comprueba si `host` tiene el formato correcto,
 * [0-255].[0-255].[0-255].[0-255].
 * @param host IP de la que chequear el formato;
 * 
 * @return 1 si falla, 0 si es correcta
 */
static int isWrongHost(const std::string& host)
{
    std::vector<std::string> octets;
    std::string octet;
    std::istringstream hostStream(host);

    while (std::getline(hostStream, octet, '.'))
	{
        octets.push_back(octet);
	}
    if (octets.size() != 4)
	{
        return ( 1 );
    }
    for (size_t i = 0; i < octets.size(); ++i)
	{
        int value = std::atoi(octets[i].c_str());
        if (value < 0 || value > 255)
		{
            return ( 1 );
		}
    }
    return ( 0 );
}

/** Comprueba si `port` tiene el formato correcto,
 * un numero entre 0 y 65535.
 * @param port Valor a chequear.
 * 
 * @return 1 si falla, 0 si es correcta
 */
static int isWrongPort(const std::string& port) 
{
    int value = std::atoi(port.c_str());
    return ( value < 0 || value > 65535 );
}