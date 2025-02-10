/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:38:43 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/10 17:31:05 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <map>
#include <vector>

class AConfig
{
	private:
		std::map<int, std::string>	err_map;
		int				client_max_body;
		
	protected:

	public:
		AConfig( void );
		AConfig( AConfig const &src );
		virtual ~AConfig( void );

		AConfig	&operator=( AConig const &src );

		
}
