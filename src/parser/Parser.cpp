//#CABECERA

#include "Parser.hpp"

std::map<std::string, t_directive>	Parser::_directives;

/**
 * Función no miembro para iniciar el mapeo de directivas permitidas en el proyecto,
 * junto con las características de cada directiva: número de argumentos, ambito de declaración
 * tipo de directiva.
 */
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
Parser::Parser( void ): _configFile( "./webserv.conf" ){}

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

/** Función para parsear el fichero de configuración.
 * @param loc Vector donde se cargarán todos los objetos Location
 * 		generados del parseo de fichero de configuración. Habra 
 * 		un objeto `Location`por cada `location` que salga en el fichero
 * 		de configuración, uno extra por cada `server` que no tenga 
 * 		declaradas `location`s en su interior.
 */
void 	Parser::parseConfigFile( std::vector<Location> &loc )
{
	std::string str;
	if ( !this->_configFile.is_open() )
		throw Parser::ParsingException( "Failure on file opening");
	std::cout << "File opened!\n"
		<< "Character count: " << this->_configFile.gcount() << std::endl;
	while ( std::getline(this->_configFile, str) )
		this->_cleanComments(str);
	this->_configFile.close();
	this->_forbidenCharsCheck()._tokenizeConfig()._processTokens( loc );
}

/** Función que va analizando los tokens almacenados en this->_tokens.
 * se considera token a un conjunto de caracteres entre espacios.
 * @param loc Vector donde se cargarán todos los objetos Location
 * 		generados del parseo de fichero de configuración. Habra 
 * 		un objeto `Location`por cada `location` que salga en el fichero
 * 		de configuración, uno extra por cada `server` que no tenga 
 * 		declaradas `location`s en su interior.
 * 
 */
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
	std::cout << "Resultado final." << std::endl;
	for ( std::vector<Location>::const_iterator it = loc.begin(); \
		it != loc.end(); it++ )
		std::cout << *it << std::endl;
}

/** Funcion auxulia para construir items t_directive
 * @param args Número de argumentos aceptados por una directiva.
 * @param context Contexto en el que puede utilizarse una directiva, consultar
 * 		la enumeración `t_context` del header para ver opciones.
 * @param type Indica el tipo de directiva, enumeración `t_type` del header.
 * 
 * @return La directiva construida.
 */
t_directive	build_directive( int args, unsigned int context, t_type type )
{
	t_directive	dir;

	dir.args = args;
	dir.context = context;
	dir.type = type;
	return ( dir );
}

/** Procesa los toques localizados dentro de una directiva `server`
 * añadiendo objetos Location al vector de localizaciones `locs`.
 *  @param loc Referencia a vector donde se cargarán todos los objetos `Location`
 * 		generados del parseo de fichero de configuración. Habra 
 * 		un objeto `Location`por cada `location` que salga en el fichero
 * 		de configuración, uno extra por cada `server` que no tenga 
 * 		declaradas `location`s en su interior.
 * @param v_str Vector de tokens a procesar. Comenzando desde la primera
 * 		palabra `server` tras procesamientos previos.
 * 
 * @return entero con el número de tokens procesados para continuar siguientes
 * 		pasos a partir del último token procesado.
 * 
*/
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
		std::cout << "\033[1;38;5;226m" << "WARNING - Empty server ignored." << "\033[0m" << std::endl;
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

/** Lógica de tratamiento e inclusión de cada una de las directivas
 * disponibles dentro de la directiva `server` en el objeto Location
 * recibido a través del argumento `server`
 * @param server Objeto `Location` en el que cargar la directiva procesada.
 * @param it	Iterador de tokens que apunta a la palabra identificadora de la directiva.
 */
void					Parser::_handleServerDirective(	Location &server, tokenIter &it )
{
	if ( "server_name" == *it \
		&& this->_checkUniqueDirective( *it, !server.getServerName().empty()) )
		server.setServerName( *(++it) );
	else if ( "listen" == *it \
		&& this->_checkUniqueDirective( *it, !server.getListen().empty()) )
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
		server.setAlias( *(++it) );
	it++;	
}

/** Lógica de tratamiento e inclusión de cada una de las directivas
 * disponibles dentro de la directiva `location` en el objeto Location
 * recibido a través del argumento `location`
 * @param location Objeto `Location` en el que cargar la directiva procesada.
 * @param it	Iterador de tokens que apunta a la palabra identificadora de la directiva.
 * @param end	Iterador que apunta al final del bloque `location` para saber cuando
 * 		parar el procesamiento del bloque
 */
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
