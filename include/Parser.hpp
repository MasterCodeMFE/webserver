//#CABECERA

#ifndef PARSER_CLASS_HPP
# define PARSER_CLASS_HPP

# include <stdexcept>
# include <cstring>
# include <string>
# include <vector>

# include "Config.hpp"

class Config;

typedef enum e_contexts
{
	E_GLOBAL	= 1 << 0,
	E_SERVER	= 1 << 1,
	E_LOCATION	= 1 << 2
}	t_context;

typedef enum e_type
{
	E_BLOCK,
	E_DIRECTIVE
}	t_type;

typedef struct s_directive
{
	int				arguments;
	unsigned int	context;
	t_type			type;

}	t_directive;

t_directive	build_directive( int args, unsigned int context, t_type type );

class Parser
{
	private:
		std::ifstream				_configFile;
		std::stringstream			_cleanedConfigFile;
		std::vector<std::string>	_tokens;
		

		Parser( Parser const &src);
		
		Parser	&operator=( Parser const &src );
		
		bool 	_isBetweenQuotes( std::string const str, size_t pos ) const;
		Parser	&_closedQuotesCheck( void );
		Parser	&_forbidenCharsCheck( void );
		Parser	&_cleanComments( std::string str );
		Parser	&_tokenizeConfig( void );
		Parser	&_processTokens( Config  &conf );
		
	public:
		Parser( void ); //Parser construidor con config_file por defecto
		~Parser( void );
		
		Parser	&setConfigFile( const char* config_file_path );
		void	parseConfigFile( Config  &conf );
		class ParsingException: public std::logic_error
		{
			public:
				ParsingException ( std::string const &msg );
		};	
};

#endif