/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:34 by manufern          #+#    #+#             */
/*   Updated: 2025/03/06 19:12:50 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int main( int argc, const char **argv )
{
	std::vector<Location> loc;
    listenSet             listeners;

    try
	{
		if (argc == 2)
			Parser().setConfigFile(argv[1]).parseConfigFile(loc);
		else
			Parser().parseConfigFile(loc);
	}
	catch ( Parser::ParsingException &e)
	{
		std::cout << RED << "PARSING ERROR: " << e.what() << DEFAULT << std::endl;
	}
    //listeners = getListeners( loc );
    DeployServer server(loc);

    server.initialize_server_sockets();
    //initialize_server_sockets(listeners);
    

    return 0;
}
