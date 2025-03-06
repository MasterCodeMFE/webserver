/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:03 by manufern          #+#    #+#             */
/*   Updated: 2025/03/06 19:12:04 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string Request::handle_delete(const HttpRequest& httpRequest)
{
    // Construye la ruta del archivo a eliminar dentro del directorio "www"
    std::string filepath = "www" + httpRequest.path;

    // Verifica si el archivo existe antes de intentar eliminarlo
    if (access(filepath.c_str(), F_OK) != 0)
    {
        std::string error = "Archivo no existe\n";
        return Request::build_http_response(error, "text/plain", 404);
    }

    // Intenta eliminar el archivo y verifica si la operación fue exitosa
    if (remove(filepath.c_str()) != 0)
    {
        std::string error = "Error al eliminar el archivo\n";
        return Request::build_http_response(error, "text/plain", 500);
    }

    // Devuelve una respuesta de éxito si el archivo se eliminó correctamente
    std::string success = "Archivo eliminado con éxito\n";
    return Request::build_http_response(success, "text/plain", 200);
}