/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:54:05 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 09:36:55 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <map>
# include <vector>
# include <string>
# include <cctype>
# include <cstdlib>
# include "Status.hpp"
# include "utils.hpp"

class Location
{
	private:
		std::vector<Status*>				_v_status_pages;
		unsigned int						_client_max_body_size;
		std::vector<std::string>			_v_methods;	
		std::map<std::string, std::string>	_m_redirections;
		bool								_autoindex;
		std::string							_index;
		std::string							_cgi;
		std::string							_root;

		Location( Location const &src );
		Location	&operator=( Location const &src );
		bool		_isDigit( std::string size );

	public:
		Location( void );
		~Location( void );

		std::vector<Status *> const					&getVStautsPages( void ) const;
		unsigned int	const						&getClienteMaxBodySize( void ) const;
		std::vector<std::string> const				&getVMethos( void ) const;
		std::map<std::string, std::string> const	&getMRedirections( void ) const;
		bool const									&getAutoindex( void );
		std::string const							&getIndex( void );
		std::string const							&getCgi( void );
		std::string const							&getRoot( void );

		void										addStatusPage( int status_code, std::string page_path);
		void										setClienteMaxBodySize( std::string size );
		void										addVMethod( std::string method );
		void										addMRedirection( std::string from, std::string to);
		void										setAutoindex( bool autoindex );
		void										setIndex( std::string index );
		void										setCgi( std::string cgi );
		void										setRoot( std::string root );
};

//std::ostream	&operator<<( std::ostream &o, Status &src);