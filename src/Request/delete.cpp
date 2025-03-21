/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:03 by manufern          #+#    #+#             */
/*   Updated: 2025/03/21 14:19:19 by manufern         ###   ########.fr       */
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

    // Eliminar la barra inicial (si existe)
    if (filename[0] == '/') {
        filename = filename.substr(1);
    }

    // Construir la ruta completa
    std::string file_path = filename;
    std::cout << "Buscando archivo en: " << file_path << std::endl;

    // Verificar si el archivo existe
    struct stat buffer;
    if (stat(file_path.c_str(), &buffer) != 0) {
        std::cerr << "Error al verificar archivo: " << strerror(errno) << std::endl;
        return location.getErrorPage(404);
    }

    // Intentar eliminar el archivo
    if (remove(file_path.c_str()) != 0) {
        std::cerr << "Error al eliminar archivo: " << strerror(errno) << std::endl;
        return location.getErrorPage(500);
    }

    std::cout << "✅ Archivo eliminado correctamente: " << filename << std::endl;
    return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nArchivo eliminado con éxito";
}
    