/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeployServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:34 by manufern          #+#    #+#             */
/*   Updated: 2025/03/12 16:54:11 by manufern         ###   ########.fr       */
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
        std::cout << "listening: " << it->first << " server name: " << it->second << std::endl;
}

DeployServer::~DeployServer( void ){}

DeployServer    &DeployServer::operator=( DeployServer const &src ){ ( void )src; return( *this ); }

int             DeployServer::_dispatch_http_request(int client_fd, HttpRequest& httpRequest )
{
    std::string response;
    Location location;

    location = findLocation(httpRequest, this->locations);
    bool is_valid_method = location.getSMethods().find(httpRequest.method) != location.getSMethods().end();    

    std::cout << location << std::endl;

    /**
     * Composición de la ruta/fichero a procesar:
     * - Si existe un `alias` en `location` del config file que va a manejar la petición 
     *      se debe añadir a la ruta base "./www/" el alias y el restante su la sustitución.
     * - Si no existe `alias` se añade el `root` de la `location` delante de la ruta solicitada
     *      en la petición. Por defecto el atributo `root`se almacena en locatión precedido 
     *      por la ruta base "./www/", por lo que si no existe root en una `location`
     *      location devuelve "./www/". Consultar el fichero "src/Server.cpp", función "setRoot"
     *      para entender la implementación.
     */ 
    if (!location.getAlias().empty())
    {
        httpRequest.path = "./www/" + location.getAlias() + httpRequest.path.substr(location.getPath().size());
    }
    else
    {
        httpRequest.path = location.getRoot() + httpRequest.path;
    }
    // Verificar si la solicitud es para un script CGI
    if (httpRequest.path.find("/cgi-bin/") == 0)
    {
        httpRequest.path = Request::handle_cgi("." + httpRequest.path, httpRequest.query_string, location);
        std::cout << "Alias: " << httpRequest.path << std::endl;
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
    std::cout << "\U0001F4E4 Enviando respuesta...\n";
    Request::send_all(client_fd, response.c_str(), response.size());

    return 0;
}
