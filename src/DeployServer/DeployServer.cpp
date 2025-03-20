/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeployServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:34 by manufern          #+#    #+#             */
/*   Updated: 2025/03/20 13:57:19 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "DeployServer.hpp"
#include "Request.hpp"


DeployServer::DeployServer( void ){}

DeployServer::DeployServer( DeployServer const &src ){ ( void )src;}

DeployServer::DeployServer( std::vector<Location> locations ): locations( locations )
{
    for ( std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); it ++ )
    {
        this->listeners.insert( std::make_pair( it->getListen(), it->getServerName() ) );
    }

    for ( std::set<std::pair<std::string, std::string> >::const_iterator it = listeners.begin(); it != listeners.end(); it ++ )
        std::cout << "listening: " << it->first << " server name: " << it->second << "\n\n" << std::endl;
}

DeployServer::~DeployServer( void ){}

DeployServer    &DeployServer::operator=( DeployServer const &src ){ ( void )src; return( *this ); }

int             DeployServer::_dispatch_http_request(int client_fd, HttpRequest& httpRequest )
{
    std::string response;
    Location location;

    location = findLocation(httpRequest, this->locations);
    bool is_valid_method = location.getSMethods().find(httpRequest.method) != location.getSMethods().end();    

    if (location.getRedirection().first != 0) // Verifica si hay un código de redirección
    {
        response = _handle_redirection(location.getRedirection().first,  location.getRedirection().second, location);
        Request::send_all(client_fd, response.c_str(), response.size());
        return 0;
    }

    if (!location.getAlias().empty())
    {
        httpRequest.path = location.getAlias() + httpRequest.path.substr(location.getPath().size() - 1);
    }
    else
    {
        httpRequest.path = location.getRoot() + httpRequest.path;
    }
    // Verificar si la solicitud es para un script CGI
    if (httpRequest.path.find("/cgi-bin/") == 0)
    {
        httpRequest.path = Request::handle_cgi("." + httpRequest.path, httpRequest.query_string, location);
    }
    if (!is_valid_method)
    {
        std::string error = "Método no permitido\n";
        std::clog << error << std::endl;
        response = location.getErrorPage(405);
    }
    // Manejar solicitud GET
    else if (httpRequest.method == "GET") {
        response = Request::handle_get(httpRequest, location);
    }
    // Manejar solicitud POST
    else if (httpRequest.method == "POST") {
        response = Request::handle_post(httpRequest, location);
    }
    // Manejar solicitud DELETE
    else if (httpRequest.method == "DELETE") {
        response = Request::handle_delete(httpRequest, location);
    }
    // Responder con error 405 si el método no es reconocido
    else {
        std::string error = "Método no permitido\n";
        response = location.getErrorPage(405);
    }

    // Enviar la respuesta al cliente
    Request::send_all(client_fd, response.c_str(), response.size());

    return 0;
}
