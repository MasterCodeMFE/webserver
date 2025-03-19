/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:34 by manufern          #+#    #+#             */
/*   Updated: 2025/03/10 15:39:20 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

static void handle_sigint(int signal);

int main( int argc, const char **argv )
{
    std::signal(SIGINT, handle_sigint);
	std::signal(SIGQUIT, handle_sigint);
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
    DeployServer server(loc);
    server.initialize_server_sockets();
    return 0;
}

/** Gestiona las señales que causan paradas abruptas del sistema
 * para que se ejecuten los destructores y evitar leaks.
 * @param signal Número asociado a la señal recibida.
 */
static void handle_sigint(int signal)
{
   (void) signal;
}