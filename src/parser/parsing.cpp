#include <fstream>
#include <iostream>
#include <sstream>
#include "main.hpp"
#include "Parser.hpp"

//static std::string	ft_curate_line( std::string str );
int main( int argc, const char **argv )
{
	Parser					p;

	if (argc == 2)
		p.setConfigFile(argv[1]);
	try
	{
		p.parseConfigFile();
	}
	catch ( Parser::ParsingException &e)
	{
		std::cout << RED << "PARSING ERROR: " << e.what() << DEFAULT << std::endl;
	}
	return ( 0 );
}
