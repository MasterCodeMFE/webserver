/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:48:12 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 22:49:20 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

template <typename Container>
void ft_clearContainer(Container& container)
{
    if ( false == container.empty() )
    {
	    for (typename Container::iterator it = container.begin(); it != container.end(); ++it) 
            delete *it;
        container.clear();
    }
}

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

    return ( copy );
}
