/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeployServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:34 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 19:59:02 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "DeployServer.hpp"
#include "Request.hpp"

static int string_to_int(const std::string &str);


DeployServer::DeployServer( void ){}

DeployServer::DeployServer( DeployServer const &src ){ ( void )src;}

DeployServer::DeployServer( std::vector<Location> locations ): locations( locations )
{
    for ( std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); it ++ )
    {
        this->listeners.insert( std::make_pair( it->getListen(), it->getServerName() ) );
    }

}

DeployServer::~DeployServer( void ){}

DeployServer    &DeployServer::operator=( DeployServer const &src ){ ( void )src; return( *this ); }

int             DeployServer::_dispatch_http_request(int client_fd, HttpRequest& httpRequest )
{
    std::string response;
    Location location;

       location = findLocation(httpRequest, this->locations);
    if (httpRequest.headers.find("Content-Length") != httpRequest.headers.end() && string_to_int (httpRequest.headers.at("Content-Length")) >  static_cast<int>(location.getClienteMaxBodySize()) )
    {
        response = location.getErrorPage(413);
        Request::send_all(client_fd, response.c_str(), response.size());
        return ( 0 );
    }
    bool is_valid_method = location.getSMethods().find(httpRequest.method) != location.getSMethods().end();    

    if (location.getRedirection().first != 0)
    {
        response = _handle_redirection(location.getRedirection().first,  location.getRedirection().second, location);
        Request::send_all(client_fd, response.c_str(), response.size());
        return 0;
    }

    if (!location.getAlias().empty())
    {
        httpRequest.path = location.getAlias() + httpRequest.path.substr(location.getPath().size());
    }
    else
    {
        httpRequest.path = location.getRoot() + httpRequest.path;
    }
    if (!httpRequest.path.empty()) 
    {
        std::string filename = httpRequest.path;
        size_t questionMark = filename.find('?');   
        if (questionMark != std::string::npos) {
            filename = filename.substr(0, questionMark);
        }
        if (filename.size() >= 4 && filename.compare(filename.size() - 4, 4, ".php") == 0)
        {  
            response = Request::handle_cgi(httpRequest.path, httpRequest.body, httpRequest.method, httpRequest.body, location);
            Request::send_all(client_fd, response.c_str(), response.size());
            return 0;
        }
    }
    if ( !location.getCgi().empty() && httpRequest.path.size() >= 4 \
        && httpRequest.path.compare(httpRequest.path.size() - 4, 4, ".php") != 0 \
        && httpRequest.method == "POST")
    {
        response = location.getErrorPage(400);
        Request::send_all(client_fd, response.c_str(), response.size());
        return 0;
    }
    if (!is_valid_method)
    {
        response = location.getErrorPage(405);
    }
    else if (httpRequest.method == "GET") {
        response = Request::handle_get(httpRequest, location);
    }
    else if (httpRequest.method == "POST") {
        response = Request::handle_post(httpRequest, location);
    }
    else if (httpRequest.method == "DELETE") {
        response = Request::handle_delete(httpRequest, location);
    }
    else {
        std::string error = "Método no permitido\n";
        response = location.getErrorPage(405);
    }
    if ( 0 > Request::send_all(client_fd, response.c_str(), response.size()) )
    {
        response = location.getErrorPage(503);
        Request::send_all(client_fd, response.c_str(), response.size());
    }

    return 0;
}


static int string_to_int(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    return value;
}