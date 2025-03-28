/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:03 by manufern          #+#    #+#             */
/*   Updated: 2025/03/28 10:25:48 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string Request::handle_delete(const HttpRequest& httpRequest, Location location)
{
    // Obtener el nombre del archivo desde la ruta
    std::string filename = httpRequest.path;
     
    std::cout << "Nombre del archivo recibido: " << filename << std::endl;
    // Verificar que el nombre del archivo sea válido
    if (filename.empty() || filename.find("..") != std::string::npos) {
        std::cerr << "Error: Nombre de archivo inválido." << std::endl;
        return location.getErrorPage(400);
    }

    // Construir la ruta completa
    std::string file_path = filename;
    std::cout << "Buscando archivo en: " << file_path << std::endl;

    // Verificar si el archivo existe
    if ( access(file_path.c_str(), F_OK) != 0) {
        std::cerr << "Error al verificar archivo: " << strerror(errno) << std::endl;
        return location.getErrorPage(404);
    }
    //Verifica
    if (access(file_path.c_str(), W_OK) != 0) {
        return location.getErrorPage(403);
    }

    // Intentar eliminar el archivo
    if (remove(file_path.c_str()) != 0) {
        std::cerr << "Error al eliminar archivo: " << strerror(errno) << std::endl;
        return location.getErrorPage(500);
    }

    std::cout << "✅ Archivo eliminado correctamente: " << filename << std::endl;
    std::string response_body = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nArchivo eliminado con éxito";
    return build_http_response(response_body, "text/plain", 204);
}
    