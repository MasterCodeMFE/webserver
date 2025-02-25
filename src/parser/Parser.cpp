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

	for ( tokenIter it = _tokens.begin(); \
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

void		Parser::_processTokens( void )
{
	//bool								looking_kw = true;
	t_context 				context = E_GLOBAL;
	std::vector<Location *> loc;

	tokenIter it = this->_tokens.begin();
	if ( Parser::_directives.empty())
		Parser::_setDirectives();
    while ( it != this->_tokens.end() )
	{
		this->_checkDirective( *it )._checkContext( context, *it);
		
		if ( "server" == *it )
		{
			std::vector<std::string> subvector(it, this->_tokens.end());
			it += this->_serverProcessing( loc, subvector );
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
	tokenIter									it = v_str.begin();
	std::vector<Location> 						server_locations;
	Location									server;	
	//bool	block_closed = false;

	(void)locs;
	if ( v_str.end() == ++it || *it != "{" )
		throw Parser::ParsingException("Expected space followed by `{` after `server` directive" );
	it++;	
	while ( it != v_str.end() )
	{
		this->_checkDirective( *it )
			._checkContext( E_SERVER, *it )
			._checkArgs( it, v_str.end());
		if ( *it == "location" )
		{
			std::cout << "LOCATION!" << std::endl;
			break;
		}
		else
		{
			this->_handleServerDirective( server, it);
		}
		it ++;		
	}
	std::cout << server << std::endl;
	return ( 0 );
}

void					Parser::_handleServerDirective( Location &server, tokenIter &it)
{
	if ( "server_name" == *it )
		server.setServerName( *(++it) );
	else if ( "listen" == *it )
		server.addListen( *(++it) );
	else if ( "error_page" == *it )
	{
		server.addStatusPage( *(it + 1), *(it + 2) );
		it += 2;
	}
	else if ( "client_max_body_size" == *it )
		server.setClienteMaxBodySize( *(++it) );
	else if ( "method" == *it )
		std::cout << "METHOD is a location directive, not server one." << std::endl;
	else if ( "redirect" == *it )
	{
		server.addMRedirection( *(it + 1), *(it + 2) );
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
		std::cout << "ALIAS is a location directive, not server one." << std::endl;
	std::cout << "ENTRA" << std::endl;
	it++;	
}



 /*
std::vector<std::string> getServerTokens( std::vector<std::string>::const_iterator it )
{
	
	it.find( "}");
}*/

