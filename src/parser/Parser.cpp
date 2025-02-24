//#CABECERA

#include "Parser.hpp"

std::map<std::string, t_directive>	Parser::_directives;

void	Parser::_setDirectives( void )
{
	_directives["server"] = build_directive( KW_SERVER, 0, E_GLOBAL, E_BLOCK );
	_directives["server_name"] = build_directive( KW_SERVER_NAME, 1, E_SERVER, E_DIRECTIVE );
	_directives["listen"] = build_directive( KW_LISTEN, 1, E_SERVER, E_DIRECTIVE );
	_directives["error_page"] = build_directive( KW_ERROR_PAGE, 2, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["client_max_body_size"] = build_directive( KW_CLIENT_MAX_BODY_SIZE, 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["location"] = build_directive( KW_LOCATION, 1, E_SERVER, E_BLOCK );
	_directives["method"] = build_directive( KW_METHOD, 1, E_LOCATION, E_DIRECTIVE );
	_directives["redirect"] = build_directive( KW_REDIRECT, 2, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["autoindex"] = build_directive( KW_AUTOINDEX, 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["index"] = build_directive( KW_INDEX, 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["cgi"] = build_directive( KW_CGI, 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["root"] = build_directive( KW_ROOT, 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["alias"] = build_directive( KW_ALIAS, 1, E_LOCATION, E_DIRECTIVE );
}

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
	this->_forbidenCharsCheck()._tokenizeConfig()._processTokens();
}

/** Excepciónes de parseo */
Parser::ParsingException::ParsingException ( std::string const &msg ): std::logic_error(msg){}

void		Parser::_processTokens( const t_context &context)
{
	std::map<std::string, t_directive>	directives;
	//bool								looking_kw = true;

	std::vector<std::string>::const_iterator it = this->_tokens.begin();
    while ( it != this->_tokens.end() )
	{
		std::ostringstream oss;
		if ( Parser::_directives.end() == Parser::_directives.find( *it ))
		{
            oss << "Unknown directive `" << *it << "`";
            throw Parser::ParsingException(oss.str());
        }

		if ( !( context & Parser::_directives[ *it ].context ) )
		{
			oss << "Directive `" << *it 
			<< "` declared out of its available scope. Check README.md in src/parsing/ route for details.";
			throw Parser::ParsingException(oss.str());
		}
		


		it ++;
	}
}

t_directive	build_directive( t_keywords kw, int args, unsigned int context, t_type type )
{
	t_directive	dir;

	dir.id = kw;
	dir.args = args;
	dir.context = context;
	dir.type = type;
	return ( dir );
}