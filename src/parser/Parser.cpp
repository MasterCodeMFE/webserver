//#CABECERA

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
	_directives["redirect"] = build_directive( 2, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["autoindex"] = build_directive( 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["index"] = build_directive( 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["cgi"] = build_directive( 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["root"] = build_directive( 1, E_SERVER | E_LOCATION, E_DIRECTIVE );
	_directives["alias"] = build_directive( 1, E_LOCATION, E_DIRECTIVE );
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

void		Parser::_processTokens( void )
{
	//bool								looking_kw = true;
	t_context 				context = E_GLOBAL;
	std::vector<Location *> loc;

	std::vector<std::string>::iterator it = this->_tokens.begin();
	if ( Parser::_directives.empty())
		Parser::_setDirectives();
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
		if ( "server" == *it )
		{
			std::vector<std::string> subvector(it, this->_tokens.end());
			it += this->_serverProcessing( loc, subvector );
			context = E_SERVER;
		}

		std::cout << *it << std::endl;
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

int	Parser::_serverProcessing( std::vector<Location *> &locs, \
			std::vector<std::string> v_str )
{
	size_t	processed_counter = 0;
	bool	block_closed = false;

	end_server_block = std::find(v_str.begin(), v_str.end(), "server");
	if ( v_str.size() < 2 || v_str.at(1) != "{")
		throw Parser::ParsingException("Expected space followed by `{` after `server` directive" );
	
/*	else if ( end_server_block == v_str.end() )
	{
		end_server_block = std::find_end(v_str.begin(), v_str.end(), "server")
	}*/
	if ( locs.empty() )
		return ( processed_counter );
	return ( processed_counter );
}
 /*
std::vector<std::string> getServerTokens( std::vector<std::string>::const_iterator it )
{
	
	it.find( "}");
}*/