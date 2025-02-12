/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:48:12 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 22:49:20 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

bool    ft_isDigit( std::string size )
{
	for (std::string::const_iterator it = size.begin(); it != size.end(); ++it) {
        if (!std::isdigit(*it)) {
            return false;
        }
    }
	return ( true );
}