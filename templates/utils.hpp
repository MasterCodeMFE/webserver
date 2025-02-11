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