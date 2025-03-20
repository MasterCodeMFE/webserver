    /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   findLocation.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:32:26 by manufern          #+#    #+#             */
/*   Updated: 2025/03/10 19:01:39 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeployServer.hpp"
#include "Request.hpp"

std::string resolveHostnameToIP(const std::string &hostname);

Location findLocation(const HttpRequest &httpRequest, std::vector<Location> locations) {
    std::string path(httpRequest.path);
    std::string host;
    try
    {
        host = httpRequest.headers.at("Host");
    }
    catch(const std::out_of_range& e)
    {
        host = "127.0.0.1:8080";
    }
    if ( path[0] != '/' )
	    path = "/" + path;
    std::cout << "host: " << host << std::endl;
    // Obtener la IP y el puerto del host
   
    

     //Añadir la ruta completa primero
    std::vector<std::string> pathSegments;
    pathSegments.push_back(path);

    // Dividir la ruta en segmentos eliminando el último directorio en cada iteración
    while ( !path.empty() && path != "/") {
        std::size_t pos = path.find_last_of('/');
        if (pos == 0) {
            path = "/";
        } else {
            path = path.substr(0, pos);
        }
        pathSegments.push_back(path);
    }

    // Crear la cadena de listen

    // Imprimir segmentos de la ruta para depuración
    std::cout << "Segmentos de la ruta:" << std::endl;
    for (std::vector<std::string>::const_iterator it = pathSegments.begin(); it != pathSegments.end(); ++it) {
        std::cout << *it << std::endl;
    }

    // Buscar la primera Location que coincide con los segmentos de la ruta y el host
    for (std::vector<std::string>::const_iterator segmentIt = pathSegments.begin(); segmentIt != pathSegments.end(); ++segmentIt) {
        /*std::cout << "--------------------------------" << std::endl; // Imprimir la ruta actual que se busca
        std::cout << "Buscando en la ruta: " << *segmentIt << std::endl; // Imprimir la ruta actual que se busca
        */
        for (std::vector<Location>::const_iterator locIt = locations.begin(); locIt != locations.end(); ++locIt) {
            std::string listen(host);
            if (isspace(host[host.size() - 1])) //eliminar si el ultimo es espacio
                listen = host.substr(0, host.size() - 1);
            // Comparar el segmento de la ruta con la Location
            if( !(*segmentIt).compare( locIt->getPath() ) && !listen.compare( locIt->getListen() ))
            {
                std::cout << "encontrado" << std::endl;
                return *locIt;
            }
            if( !(*segmentIt + "/").compare( locIt->getPath() ) && !listen.compare( locIt->getListen() ))
            {
                std::cout << "encontrado" << std::endl;
                return *locIt;
            }
        }
        std::cout << std::endl; // Separar cada iteración de segmentos
    }

    std::string domain = host.substr(0, host.find(':'));

    // Resolver el dominio a una IP
    std::string resolvedIP = resolveHostnameToIP(domain);
    if (!resolvedIP.empty()) {
        host = resolvedIP + host.substr(host.find(':')); // Reemplazar el nombre con la IP
    }

    std::cout << "Host después de resolución: " << host << std::endl;

    if ( path[0] != '/' )
	    path = "/" + path;
    std::cout << "host: " << host << std::endl;
    // Obtener la IP y el puerto del host

    // Imprimir segmentos de la ruta para depuración
    std::cout << "Segmentos de la ruta:" << std::endl;
    for (std::vector<std::string>::const_iterator it = pathSegments.begin(); it != pathSegments.end(); ++it) {
        std::cout << *it << std::endl;
    }

    // Buscar la primera Location que coincide con los segmentos de la ruta y el host
    for (std::vector<std::string>::const_iterator segmentIt = pathSegments.begin(); segmentIt != pathSegments.end(); ++segmentIt) {
        /*std::cout << "--------------------------------" << std::endl; // Imprimir la ruta actual que se busca
        std::cout << "Buscando en la ruta: " << *segmentIt << std::endl; // Imprimir la ruta actual que se busca
        */
        for (std::vector<Location>::const_iterator locIt = locations.begin(); locIt != locations.end(); ++locIt) {
            std::string listen(host);
            if (isspace(host[host.size() - 1])) //eliminar si el ultimo es espacio
                listen = host.substr(0, host.size() - 1);
            // Comparar el segmento de la ruta con la Location
            if( !(*segmentIt).compare( locIt->getPath() ) && !listen.compare( locIt->getListen() ))
            {
                std::cout << "encontrado" << std::endl;
                return *locIt;
            }
            if( !(*segmentIt + "/").compare( locIt->getPath() ) && !listen.compare( locIt->getListen() ))
            {
                std::cout << "encontrado" << std::endl;
                return *locIt;
            }
        }
        std::cout << std::endl; // Separar cada iteración de segmentos
    }

    // Si no se encuentra ninguna, devuelve un Location vacío
    std::cout << "No se encontró ninguna Location coincidente." << std::endl; // Mensaje de no encontrado
    return Location();
}

std::string resolveHostnameToIP(const std::string &hostname) {
    struct addrinfo hints;
    struct addrinfo *res;

    // Inicializar `hints` manualmente
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;  // Solo IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    if (getaddrinfo(hostname.c_str(), NULL, &hints, &res) != 0) {
        std::cerr << "Error resolviendo hostname: " << hostname << std::endl;
        return hostname; // Devuelve el mismo nombre si no se puede resolver
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    unsigned char *ip = (unsigned char *)&(ipv4->sin_addr);

    // Convertir la IP a string manualmente
    std::ostringstream ipStr;
    ipStr << (int)ip[0] << "." << (int)ip[1] << "." << (int)ip[2] << "." << (int)ip[3];

    freeaddrinfo(res); // Liberar memoria

    return ipStr.str();
}

