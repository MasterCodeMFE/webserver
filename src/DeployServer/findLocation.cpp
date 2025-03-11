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

Location findLocation(const HttpRequest &httpRequest, std::vector<Location> locations) {
    std::string path(httpRequest.path);
    std::string host = httpRequest.headers.at("Host");

    std::cout << "host: " << host << std::endl;
    // Obtener la IP y el puerto del host
   
    

     //Añadir la ruta completa primero
    std::vector<std::string> pathSegments;
    pathSegments.push_back(path);

    // Dividir la ruta en segmentos eliminando el último directorio en cada iteración
    while (path != "/") {
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
        }
        std::cout << std::endl; // Separar cada iteración de segmentos
    }

    // Si no se encuentra ninguna, devuelve un Location vacío
    std::cout << "No se encontró ninguna Location coincidente." << std::endl; // Mensaje de no encontrado
    return Location();
}
