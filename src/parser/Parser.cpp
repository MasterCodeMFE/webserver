//#CABECERA

#include "Parser.hpp"

/** Constructor por defecto.
 * Al no especificarse un fichero de configuración, se indica la ruta del
 * fichero de configuración por defecto para su procesamineto.
 */
Parser::Parser( void ): _configFile( "./src/parser/webserv.conf" ){}

/** Copy constructor. Al ser privado, no se utilizará, por lo tanto tampoco
 * definimos su comportamiento.
 */
Parser::Parser( Parser const &src){ (void)src; }

/** Destructor */
Parser::~Parser( void ){}

/** Copy assignment operator. Al ser privado, no se utilizará, por lo tanto tampoco
 * definimos su comportamiento.
 */
Parser	&Parser::operator=( Parser const &src ) { ( void )src; return ( *this ); }

/** Checkea si un caracter en la posición `char_index` se encuentra entre comillas o no.*/
bool	Parser::_isBetweenQuotes( std::string const str, size_t const char_index ) const
{
	bool	between_quotes;
	size_t	open_quote;
	size_t	close_quote;

	between_quotes = false;
	open_quote = str.find( '"');
	close_quote = str.find( '"', open_quote + 1);
	if ( open_quote != std::string::npos && close_quote == std::string::npos)
		throw Parser::ParsingException( "Unclosed quotes.");
	if ( char_index < open_quote )
		return ( false );
	else if ( char_index > open_quote && char_index < close_quote )
		return ( true );
	else 
		between_quotes = this->_isBetweenQuotes( str.substr( close_quote + 1), \
			char_index - close_quote - 1 );
	return ( between_quotes );
}

void	Parser::_closedQuotesCheck( void )
{
	size_t	quotes_counter;
	size_t	pos; 

	quotes_counter = 0;
	pos = this->_cleanedConfigFile.str().find('"', 0);
	while ( pos != std::string::npos )
	{
		quotes_counter ++;
		pos = this->_cleanedConfigFile.str().find('"', pos + 1);
	}
	if ( quotes_counter % 2 )
		throw Parser::ParsingException( "Unclosed quotes.");
}

/** Metodo que elimina comentarios de un string recibido si existen.
 * Un comentario es todo texto posterior a un signo `#`, siempre y cuando este
 * fuera de un rango entre comillas dobles `"`, en otro caso se ignorará.
 * @param str Strign a procesar, normalmente una linea leida de un fichero con
 * getline.
 * 
 * @return Añade `str` sin comentarios en el atributo `_cleanedConfigFile` y
 * 	devuelve una referencia a si mismo para poder encadenar metodos del objeto.
 */
Parser	&Parser::_cleanComments( std::string str )
{
	size_t	haystack = str.find( '#' );

	if ( haystack != std::string::npos \
			&& !this->_isBetweenQuotes( str, haystack) )
		str = str.substr( 0, haystack);
	this->_cleanedConfigFile << str;
	if ( !str.empty() )
		this->_cleanedConfigFile << '\n';
	return( *this );
}

/** Convierte `_cleanedConfigFile` en tokens */
Parser	&Parser::_tokenizeConfig( void )
{
	char		*cleaned_str;
	const char	*delimiters;
	char		*tokens;
	
	cleaned_str = new char[strlen(this->_cleanedConfigFile.str().c_str()) + 1];
	delimiters = std::string(" \t\f\v\n\r").c_str();
	strcpy(cleaned_str, this->_cleanedConfigFile.str().c_str());
	tokens = strtok(cleaned_str, delimiters);
	while ( tokens != NULL )
	{
		std::cout << "TOKEN " << tokens << std::endl;
		tokens = strtok(NULL, delimiters);
	}
	delete cleaned_str;
	//std::cout << "Tokenizing..." << std::endl;
	return ( *this );
}

Parser	&Parser::setConfigFile( const char* config_file_path )
{
	this->_configFile.open( config_file_path );
	return ( *this );
}

void 	Parser::parseConfigFile( Config &conf)
{
	( void )conf;
	std::string str;
	if ( !this->_configFile.is_open() )
		throw Parser::ParsingException( "Failure on file opening");
	std::cout << "File opened!\n"
		<< "Character count: " << this->_configFile.gcount() << std::endl;
	while ( std::getline(this->_configFile, str) )
	{
		this->_cleanComments(str);
	}
	this->_closedQuotesCheck();
	std::cout << this->_cleanedConfigFile.str() << std::endl;
	this->_tokenizeConfig();
}
	
Parser::ParsingException::ParsingException ( std::string const &msg ): std::logic_error(msg){}