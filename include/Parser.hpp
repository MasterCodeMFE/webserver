//#CABECERA

#ifndef PARSER_CLASS_HPP
# define PARSER_CLASS_HPP

# include <stdexcept>
# include <cstring>
# include <string>
# include <vector>
# include <map>
# include <sstream>
# include <fstream>
# include <iostream>
# include "Location.hpp"

typedef enum e_contexts
{
	E_GLOBAL	= 0b001,
	E_SERVER	= 0b010,
	E_LOCATION	= 0b100
}	t_context;

typedef enum e_type
{
	E_BLOCK,
	E_DIRECTIVE
}	t_type;

typedef enum e_keywords
{
	KW_SERVER,
	KW_SERVER_NAME,
	KW_LISTEN,
	KW_ERROR_PAGE,
	KW_CLIENT_MAX_BODY_SIZE,
	KW_LOCATION,
	KW_METHOD,
	KW_REDIRECT,
	KW_AUTOINDEX,
	KW_INDEX,
	KW_CGI,
	KW_ROOT,
	KW_ALIAS
} t_keywords;


typedef struct s_directive
{
	t_keywords		id;
	int				args;
	unsigned int	context;
	t_type			type;
}	t_directive;


t_directive	build_directive( t_keywords kw, int args, unsigned int context, t_type type );

class Parser
{
	private:
		std::ifstream								_configFile;
		std::stringstream							_cleanedConfigFile;
		std::vector<std::string>					_tokens;
		static std::map<std::string, t_directive>	_directives;
		

		Parser( Parser const &src);
		
		Parser	&operator=( Parser const &src );
		
		Parser					&_forbidenCharsCheck( void );
		Parser					&_cleanComments( std::string str );
		Parser					&_tokenizeConfig( void );
		void					_processTokens( const t_context &context =  E_GLOBAL);
		// void					_processTokens( const t_context &context =  E_GLOBAL, std::vector<Location> &config);

		static void				_setDirectives( void );
		
	public:
		Parser( void ); //Parser construidor con config_file por defecto
		~Parser( void );
		
		Parser	&setConfigFile( const char* config_file_path );
		void	parseConfigFile( void );
		class ParsingException: public std::logic_error
		{
			public:
				ParsingException ( std::string const &msg );
		};	
};

#endif