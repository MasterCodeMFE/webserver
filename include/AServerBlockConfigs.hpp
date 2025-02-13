/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerBlockConfigs.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:53:43 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/13 16:53:46 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASERVER_BLOCK_CONFIGS_CLASS_HPP
# define ASERVER_BLOCK_CONFIGS_CLASS_HPP

# include <string>
# include <map>


class AServerBlockConfigs
{
	protected:
		std::map<std::string, std::string>	_m_redirections;

	public:
		AServerBlockConfigs( void );
		AServerBlockConfigs( AServerBlockConfigs const &src );
		virtual ~AServerBlockConfigs( void ) = 0;

		AServerBlockConfigs	&operator=( AServerBlockConfigs const &src );

		std::map<std::string, std::string> const	&getMRedirections( void ) const;

		void										addMRedirection( std::string from, std::string to);
};

#endif
