//#CABECERA

#ifndef PARSER_CLASS_HPP
# define PARSER_CLASS_HPP

# include <stdexcept>
# include <cstring>
# include <string>

# include "Config.hpp"

class Config;


class Parser
{
	private:
		std::ifstream		_configFile;
		std::stringstream	_cleanedConfigFile;
		//std::string[]		tokens;
		

		Parser( Parser const &src);
		
		Parser	&operator=( Parser const &src );
		
		bool 	_isBetweenQuotes( std::string const str, size_t pos ) const;
		void	_closedQuotesCheck( void );
		Parser	&_cleanComments( std::string str );
		Parser	&_tokenizeConfig(void);
		
		public:
		Parser( void ); //Parser construidor con config_file por defecto
		Parser( const char* config_file_path ); //Parser construidor con config_file especifico
		~Parser( void );
		
		Parser	&setConfigFile( const char* config_file_path );
		void	parseConfigFile( Config  &conf);
		class ParsingException: public std::logic_error
		{
			public:
				ParsingException ( std::string const &msg );
		};	
};

#endif