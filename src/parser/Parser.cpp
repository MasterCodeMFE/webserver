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

	if ( haystack != std::string::npos )
		str = str.substr( 0, haystack);
	this->_cleanedConfigFile << str;
	if ( !str.empty() )
		this->_cleanedConfigFile << '\n';
	return( *this );
}

/** Convierte `_cleanedConfigFile` en tokens separados por los `delimiters`indicados.*/
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

	for ( std::vector<std::string>::iterator it = _tokens.begin(); \
		it != _tokens.end(); it ++ )
		std::cout << *it << " | ";
	std::cout << std::endl;

	delete[] cleaned_str;
	return ( *this );
}

/** Carga el fichero especificado como primer argumento después del nombre del programa
 * como fichero de configuración a tratar.
 * @param config_file_path Ruta al fichero de configuración.
 * 
 * @return Carga el fichero en el atributo `_configFile` y retorna una referencia al objeto
 * para poder encadenar metodos de la clase en caso de necesitarlo.
 */
Parser	&Parser::setConfigFile( const char* config_file_path )
{
	this->_configFile.close();
	this->_configFile.open (config_file_path);
	return ( *this );
}

/** Función para parsear el fichero de configuración
 * @param conf Objeto Config donde cargar los valores parseados.
 */
void 	Parser::parseConfigFile( void )
{
	std::string str;
	if ( !this->_configFile.is_open() )
		throw Parser::ParsingException( "Failure on file opening");
	std::cout << "File opened!\n"
		<< "Character count: " << this->_configFile.gcount() << std::endl;
	while ( std::getline(this->_configFile, str) )
		this->_cleanComments(str);
	this->_configFile.close();
	this->_forbidenCharsCheck()._tokenizeConfig();
}

/** Excepciónes de parseo */
Parser::ParsingException::ParsingException ( std::string const &msg ): std::logic_error(msg){}

Parser	&Parser::_processTokens( void )
{
	std::map<std::string, t_directive>	directives;
	//bool								looking_kw = true;
	//t_context							context = E_GLOBAL;								

	directives["server"] = build_directive(0, E_GLOBAL, E_BLOCK);
	directives["server_name"] = build_directive(1, E_SERVER, E_DIRECTIVE);
	directives["listen"] = build_directive(1, E_SERVER, E_DIRECTIVE);
	directives["error_page"] = build_directive(2, E_GLOBAL | E_SERVER | E_LOCATION, E_DIRECTIVE);
	directives["client_max_body_size"] = build_directive(1, E_GLOBAL | E_SERVER | E_LOCATION, E_DIRECTIVE);
	directives["location"] = build_directive(1, E_SERVER, E_BLOCK);
	directives["method"] = build_directive(1, E_LOCATION, E_DIRECTIVE);
	directives["redirect"] = build_directive(2, E_SERVER | E_LOCATION, E_DIRECTIVE);
	directives["autoindex"] = build_directive(1, E_GLOBAL | E_SERVER | E_LOCATION, E_DIRECTIVE);
	directives["index"] = build_directive(1, E_GLOBAL | E_SERVER | E_LOCATION, E_DIRECTIVE);
	directives["cgi"] = build_directive(1, E_GLOBAL | E_SERVER | E_LOCATION, E_DIRECTIVE);
	directives["root"] = build_directive(1, E_GLOBAL | E_SERVER | E_LOCATION, E_DIRECTIVE);
	directives["alias"] = build_directive(1, E_LOCATION, E_DIRECTIVE);
	
	/*for ( std::vector<std::string>::const_iterator it = this->_tokens.begin(); \
		it != this->_tokens.end(); it++)
	{
		try
		{
			if ( looking_kw && (directives.at( *it ).context & context ) )
			{
				
				looking_kw != looking_kw;
			}
				
		}
	}*/
	return ( *this );
}

t_directive	build_directive( int args, unsigned int context, t_type type )
{
	t_directive	dir;

	dir.arguments = args;
	dir.context = context;
	dir.type = type;
	return ( dir );
}