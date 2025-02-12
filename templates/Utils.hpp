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

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iterator>
# include <string>

template <typename T>
struct remove_pointer {
    typedef T type;
};

template <typename T>
struct remove_pointer<T*> {
    typedef T type;
};

template <typename Container>
void        ft_clearContainer(Container& container);

template <typename Container>
Container   deepCopyContainer(Container const & src);

bool        ft_isDigit( std::string size );

# include "Utils.tpp"
#endif
