#include <fstream>
#include <iostream>
#include <sstream>
#include "main.hpp"
#include "Parser.hpp"

//static std::string	ft_curate_line( std::string str );
int main( int argc, const char **argv )
{
	std::ifstream	ifs;
	std::string		str;
	std::string 	cleaned_str;
	Config			conf;
	Parser			p;

	// Traqueo de la excepción de lectura de archivo input
	// ifs.exceptions( std::ifstream::failbit );

	if (argc == 2)
	{
		try
		{
			p.setConfigFile(argv[1]).parseConfigFile(conf);
		}
		catch ( std::logic_error &e)
		{
			std::cout << RED << "ERROR: " << e.what() << DEFAULT << std::endl;
		}
		/*ifs.open( argv[1], std::ifstream::in );
		if (ifs.is_open())
		{
			std::cout << "File opened!\n"
				<< "Character count: " << ifs.gcount()
				<< std::endl;
			while ( std::getline(ifs, str) )
			{
				cleaned_str += ft_curate_line(str);
				if ( !str.empty() )
				cleaned_str += "\n\r";
			}
			std::cout << cleaned_str << std::endl;
		}
		else
		{
			std::cout << "ERROR: Failure on file opening." << std::endl;
		}*/
	}
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
/*
static std::string	ft_curate_line( std::string str )
{
	// Detectar las "" y simples y si están cerradas
	size_t	haystack;
	size_t	open_quote;
	size_t	close_quote;

	haystack = str.find( '#' );
	open_quote = str.find( '"');
	if ( haystack < open_quote )
		str = str.substr( 0, haystack);
	else
	{
		while ( open_quote != std::string::npos )
		{
			close_quote = str.find( '"', open_quote + 1);
			if ( close_quote != std::string::npos )
			{
				std::cout << "OK: Closed quotes." << std::endl;
				haystack = str.find( '#',  close_quote + 1);
				open_quote = str.find( '"', close_quote + 1);
				if ( haystack < open_quote && haystack > close_quote)
					str = str.substr( 0, haystack);
			}
			else
			{
				std::cout << "ERROR: Not closed quotes." << std::endl;
				open_quote = close_quote;
			}
		}
	}
	return (str);
}*/