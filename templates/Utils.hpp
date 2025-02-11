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

template <typename T>
struct remove_pointer {
    typedef T type;
};

template <typename T>
struct remove_pointer<T*> {
    typedef T type;
};

template <typename Container>
void ft_clearContainer(Container& container)
{
    if ( false == container.empty() )
    {
	    for (typename Container::iterator it = container.begin(); it != container.end(); ++it) 
            delete *it;
        container.clear();
    }
};

template <typename Container>
Container deepCopyContainer(Container const & src)
{
    Container copy;
    copy.reserve(src.size());

    for (typename Container::const_iterator it = src.begin(); it != src.end(); ++it) {
        if (*it)
        {
            typename Container::value_type newElement = \
                new typename remove_pointer<typename Container::value_type>::type(**it);
            copy.push_back(newElement);
        }
    }

    return copy;
};

bool    isDigit( std::string size )
{
	for (std::string::const_iterator it = size.begin(); it != size.end(); ++it) {
        if (!std::isdigit(*it)) {
            return false;
        }
    }
	return ( true );
}

#endif
