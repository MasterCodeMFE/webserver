/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:03 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 20:01:30 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string Request::handle_delete(const HttpRequest& httpRequest, Location location)
{
    std::string filename = httpRequest.path;
     
    if (filename.empty() || filename.find("..") != std::string::npos) {
        return location.getErrorPage(400);
    }

    std::string file_path = filename;
    if ( access(file_path.c_str(), F_OK) != 0) {
        return location.getErrorPage(404);
    }
    if (access(file_path.c_str(), W_OK) != 0) {
        return location.getErrorPage(403);
    }

    if (remove(file_path.c_str()) != 0) {
        std::cerr << "Error al eliminar archivo: " << strerror(errno) << std::endl;
        return location.getErrorPage(500);
    }
    std::string response_body = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nArchivo eliminado con éxito";
    return build_http_response(response_body, "text/plain", 204);
}
    