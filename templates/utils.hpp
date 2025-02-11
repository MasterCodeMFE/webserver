//CABECERA

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iterator>

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

#endif