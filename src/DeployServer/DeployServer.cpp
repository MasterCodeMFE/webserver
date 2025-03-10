/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeployServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:34 by manufern          #+#    #+#             */
/*   Updated: 2025/03/10 18:59:36 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "DeployServer.hpp"
#include "Request.hpp"


DeployServer::DeployServer( void ){}

DeployServer::DeployServer( DeployServer const &src ){ ( void )src;}

DeployServer::DeployServer( std::vector<Location> locations ): locations(locations)
{
    for ( std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); it ++ )
    {
        this->listeners.insert( std::make_pair( it->getListen(), it->getServerName() ) );
    }

    for ( std::set<std::pair<std::string, std::string> >::const_iterator it = listeners.begin(); it != listeners.end(); it ++ )
        std::cout << "listening: " << it->first << " server name: " << it->second << std::endl;
}

DeployServer::~DeployServer( void ){}

DeployServer    &DeployServer::operator=( DeployServer const &src ){ ( void )src; return( *this ); }

int             DeployServer::_dispatch_http_request(int client_fd, const HttpRequest& httpRequest )
{
    std::string response;
    Location location;

    location = findLocation(httpRequest, this->locations);

    std::cout << location << std::endl;

    // Verificar si la solicitud es para un script CGI
    if (httpRequest.path.find("/cgi-bin/") == 0) {
        response = Request::handle_cgi("." + httpRequest.path, httpRequest.query_string);
    }
    // Manejar solicitud GET
    else if (httpRequest.method == "GET") {
        response = Request::handle_get(httpRequest);
    }
    // Manejar solicitud POST
    else if (httpRequest.method == "POST") {
        response = Request::handle_post(httpRequest);
    }
    // Manejar solicitud DELETE
    else if (httpRequest.method == "DELETE") {
        response = Request::handle_delete(httpRequest);
    }
    // Responder con error 405 si el método no es reconocido
    else {
        std::string error = "Método no permitido\n";
        response = Request::build_http_response(error, "text/plain", 405);
    }

    // Enviar la respuesta al cliente
    std::cout << "\U0001F4E4 Enviando respuesta...\n";
    Request::send_all(client_fd, response.c_str(), response.size());

    return 0;
}
