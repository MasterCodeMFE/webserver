#include <fstream>
#include <iostream>
#include <sstream>
#include "main.hpp"
#include "Parser.hpp"

//static std::string	ft_curate_line( std::string str );
int main( int argc, const char **argv )
{
	Config			conf;
	Parser			p;

	if (argc == 2)
		p.setConfigFile(argv[1]);
	try
	{
		p.parseConfigFile(conf);
	}
	catch ( std::logic_error &e)
	{
		std::cout << RED << "ERROR: " << e.what() << DEFAULT << std::endl;
	}
	return ( 0 );
}
