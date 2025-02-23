/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommonConfigs.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:54:02 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/13 16:54:04 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMON_CONFIGS_CLASS_HPP
# define ACOMMON_CONFIGS_CLASS_HPP

# include <map>
# include <cstdlib>
# include "Status.hpp"
# include "Utils.hpp"

class ACommonConfigs
{
    protected:
        std::map<int, std::string>	_m_status_pages;
        unsigned int				_client_max_body_size;
        bool						_autoindex;
		std::string					_index;
		std::string					_cgi;
		std::string					_root;

        ACommonConfigs( ACommonConfigs const &src );
		
		ACommonConfigs	&operator=( ACommonConfigs const &src );

    public:
        ACommonConfigs( void );
		virtual ~ACommonConfigs( void ) = 0;

        std::map<int, std::string> const			&getMStautsPages( void ) const;
		unsigned int	const						&getClienteMaxBodySize( void ) const;
		bool const									&getAutoindex( void )const;
        std::string const							&getIndex( void )const;
		std::string const							&getCgi( void )const;
		std::string const							&getRoot( void )const;

        void										addStatusPage( int status_code, std::string page_path);
		void										setClienteMaxBodySize( std::string size );
        void										setAutoindex( bool autoindex );
		void										setIndex( std::string index );
		void										setCgi( std::string cgi );
		void										setRoot( std::string root );
};
#endif
