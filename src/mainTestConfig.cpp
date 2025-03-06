//CABECERA!!

#include "test.hpp"

static std::set<std::pair<std::string, std::string> > getListeners( std::vector<Location> locations );

int main( int argc, const char **argv )
{
	std::vector<Location> loc;
    listenSet             listeners;

    try
	{
		if (argc == 2)
			Parser().setConfigFile(argv[1]).parseConfigFile(loc);
		else
			Parser().parseConfigFile(loc);
	}
	catch ( Parser::ParsingException &e)
	{
		std::cout << RED << "PARSING ERROR: " << e.what() << DEFAULT << std::endl;
	}
    listeners = getListeners( loc );
    initialize_server_sockets(listeners);

    return 0;
}

/** Genera un set con los pares únicos de `host:port` y `server_name` 
 * existentes a partir del vector de localizaciones.
 *  
 * @param  locations Vector de localizaciones obtenidas de parsear el fichero de configuración.accept4
 * 
 * @return Set de pares `host:port` y `server_name` únicos obtenidos de analizar locations.
 */
static std::set<std::pair<std::string, std::string> > getListeners( std::vector<Location> locations )
{
    std::set<std::pair<std::string, std::string> > listeners;

    for ( std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); it ++ )
    {
        listeners.insert( std::make_pair( it->getListen(), it->getServerName() ) );
    }

    for ( std::set<std::pair<std::string, std::string> >::const_iterator it = listeners.begin(); it != listeners.end(); it ++ )
        std::cout << "listening: " << it->first << " server name: " << it->second << std::endl;
    return ( listeners );
}