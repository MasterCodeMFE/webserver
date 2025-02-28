#include <fstream>
#include <iostream>
#include <sstream>
#include "main.hpp"
#include "Parser.hpp"

//static std::string	ft_curate_line( std::string str );
int main( int argc, const char **argv )
{
	std::vector<Location> loc;

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
	return ( 0 );
}
