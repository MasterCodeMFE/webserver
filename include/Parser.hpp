/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:13:48 by manufern          #+#    #+#             */
/*   Updated: 2025/03/06 19:13:49 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_CLASS_HPP
# define PARSER_CLASS_HPP

# include <stdexcept>
# include <cstring>
# include <string>
# include <vector>
# include <iterator>
# include <map>
# include <algorithm>
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

typedef struct s_directive
{
	int				args;
	unsigned int	context;
	t_type			type;
}	t_directive;


t_directive	build_directive( int args, unsigned int context, t_type type );

class Parser
{
	typedef std::vector<std::string>::iterator		tokenIter;
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
		void					_processTokens(  std::vector<Location> &loc );
		int						_serverProcessing( std::vector<Location> &locs, \
									std::vector<std::string> v_str );

		Parser					&_checkDirective( std::string directive );
		Parser					&_checkContext( t_context current_context, std::string directive );
		Parser					&_checkArgs( tokenIter kw, tokenIter end );
		Parser					&_checkClosedBlock( tokenIter begin, tokenIter end );
		Parser					&_checkLocationLast( std::string directive, bool locationInServer );
		bool					_checkUniqueDirective( std::string directive, bool alreadyHasValue );
		Parser					&_checkUniqueLocation(  std::string path, std::vector<Location> server_locations );

		void					_handleServerDirective( Location &server, tokenIter &it );
		void					_handleLocationDirective( Location &location, tokenIter &it, tokenIter end );

		static void				_setDirectives( void );
		
	public:
		Parser( void );
		~Parser( void );
		
		Parser	&setConfigFile( const char* config_file_path );
		void	parseConfigFile( std::vector<Location> &loc );
		class ParsingException: public std::logic_error
		{
			public:
				ParsingException ( std::string const &msg );
		};	
};

#endif