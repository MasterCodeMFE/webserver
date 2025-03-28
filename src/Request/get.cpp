/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:07 by manufern          #+#    #+#             */
/*   Updated: 2025/03/28 12:24:05 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string  Request::handle_get(const HttpRequest& request, Location location)
{
    // Obtiene la ruta del archivo solicitado
    std::string filepath = Request::_get_file_path(request.path, location);

    struct stat file_stat;
    
    // Verifica si el archivo o directorio existe
    if (::stat(filepath.c_str(), &file_stat) != 0)
    {
        std::cerr << "Error: No se encontró el archivo o directorio " << filepath << std::endl;
        return (location.getErrorPage(404));
    }
    
    // Si es un directorio, genera un listado de su contenido
    if (S_ISDIR(file_stat.st_mode)  && location.getAutoindex() == true)
    {
        std::cerr << "ENTRA EN DIRECTOREIO" << std::endl;
        return Request::_listDirectory(filepath, request.path, location);
    }
    else if(S_ISDIR(file_stat.st_mode)  && location.getAutoindex() == false)
    {
        std::cerr << "Error: autoindex off."<< std::endl;
        return (location.getErrorPage(404));
    }
    
    // Verifica si el archivo tiene permisos de lectura
    if (access(filepath.c_str(), R_OK) != 0)
    {
        return Status::getErrorPage(403); // Retorna error 403 si no hay permisos
    }

    // Lee el contenido del archivo
    std::string content = Request::_read_file(filepath);
    // Obtiene el tipo de contenido (MIME type) del archivo
    std::string content_type = Request::_get_content_type(filepath);

    // Construye y retorna la respuesta HTTP con el archivo solicitado
    return build_http_response(content, content_type, 200);
}


bool containsIndexHtml(std::string directoryPath, std::string index)
{
    DIR *dir = opendir(directoryPath.c_str());
    if (!dir)
    {
        std::cerr << "No se pudo abrir el directorio: " << directoryPath << std::endl;
        return false;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (std::strcmp(entry->d_name, index.c_str()) == 0)
        {
            closedir(dir);
            return true;
        }
    }

    closedir(dir);
    return false;
}

// ========================================
//  FUNCIÓN: get_file_path
// ========================================
// Devuelve la ruta de un archivo en el servidor basada en la solicitud HTTP.
//
// Se encarga de:
// 1. Retornar la ruta del archivo solicitado dentro del directorio "www".
// 2. Si la solicitud es "/", se devuelve "www/index.html".
//
// Parámetros:
// - request_path: Ruta solicitada por el cliente.
//
// Retorno:
// - Cadena con la ruta del archivo en el sistema.
std::string Request::_get_file_path(const std::string &request_path, Location location) {
    struct stat file_stat;
    std::string index = location.getIndex();

    std::clog << index << std::endl;
    if (index.empty()) {
        index = "index.html";
    }
    std::string path = request_path;
    if (::stat(path.c_str(), &file_stat) != 0) {
        return "";
    }
    if (S_ISDIR(file_stat.st_mode)) {
        if (containsIndexHtml(path, index) == true) {
            // Evita dobles "//" en la ruta final
            if (path[path.size() - 1] != '/') {
                path += "/";
            }
            path += index;
            return path;
        }
    }

    return request_path;
}

// ========================================
//  FUNCIÓN: listDirectory
// ========================================
// Genera una lista en formato HTML con el contenido de un directorio.
//
// Se encarga de:
// 1. Intentar abrir el directorio especificado.
// 2. En caso de error, devolver una página de error 404.
// 3. Recorrer los archivos y directorios dentro del directorio dado.
// 4. Generar una lista en HTML con enlaces a cada archivo/directorio.
//
// Parámetros:
// - dirPath: Ruta del directorio a listar.
// - requestPath: Ruta de la solicitud HTTP, utilizada para construir los enlaces.
//
// Retorno:
// - Una cadena con la respuesta HTTP en formato HTML.
std::string Request::_listDirectory(const std::string &dirPath, const std::string &requestPath, Location location)
{
    // Verifica si el directorio existe y si tiene permisos de lectura
    if (access(dirPath.c_str(), F_OK) != 0) {
        return location.getErrorPage(404); // No existe el directorio
    }
    if (access(dirPath.c_str(), R_OK) != 0) {
        return location.getErrorPage(403); // No hay permisos de lectura
    }
    // Intenta abrir el directorio
    DIR *dir = opendir(dirPath.c_str());
    if (!dir) {
        return location.getErrorPage(500); // Otro error inesperado
    }    // Construye la respuesta HTML con el índice del directorio
    std::stringstream responseBody;
    responseBody << "<!DOCTYPE html>\n"
                 << "<html>\n"
                 << "<head>\n"
                 << "<meta charset=\"UTF-8\">\n"
                 << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                 << "<title>Índice de " << dirPath << "</title>\n"
                 << "<style>\n"
                 << "body { font-family: Arial, sans-serif; padding: 20px; background-color: #F4F4F4; }\n"
                 << "h1 { text-align: center; color: #333; }\n"
                 << "ul { list-style-type: none; padding: 0; max-width: 600px; margin: 20px auto; }\n"
                 << "li { background: white; margin: 10px 0; padding: 10px; border-radius: 5px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }\n"
                 << "a { text-decoration: none; color: #007BFF; font-weight: bold; display: inline-block; }\n"
                 << "a:hover { color: #0056B3; }\n"
                 << "button { margin-left: 10px; background-color: #DC3545; color: white; border: none; border-radius: 5px; cursor: pointer; }\n"
                 << "button:hover { background-color: #C82333; }\n"
                << "</style>\n"
                << "<script>\n"
                << "function deleteFile(filePath) {\n"
                << "    if (confirm('¿Estás seguro de que deseas borrar este archivo?')) {\n"
                << "        fetch(filePath, {\n"
                << "            method: 'DELETE'\n"
                << "        })\n"
                << "        .then(response => {\n"
                << "            if (response.ok) {\n"
                << "                alert('Archivo borrado con éxito.');\n"
                << "                location.reload(); // Recarga la página para actualizar la lista\n"
                << "            } else {\n"
                << "                alert('Archivo no borrable, detalles en response code.');\n"
                << "            }\n"
                << "        })\n"
                << "        .catch(error => {\n"
                << "            alert('Error al realizar la solicitud: ' + error.message);\n"
                << "        });\n"
                << "    }\n"
                << "}\n"
                << "</script>\n"       
                << "</head>\n"
                << "<body>\n"
                << "<h1>Índice de " << dirPath << "</h1>\n"
                << "<ul>\n";
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string fileName = entry->d_name;
        if (fileName == "." || fileName == "..")
            continue;
        std::string fullPath = dirPath + "/" + fileName;
        struct stat fileStat;
        // Verifica si stat() falla para evitar errores
        if (stat(fullPath.c_str(), &fileStat) != 0) {
            continue; // Si hay error al obtener información, ignora este archivo
        }

        std::string icon = "📄"; // Icono por defecto para archivos
        if (S_ISDIR(fileStat.st_mode)) {
            icon = "📁"; // Si es un directorio
        }
        // Agrega el archivo/directorio a la lista en HTML
        responseBody << "<li>" << icon << " <a href=\"" << requestPath.substr(5) + "/" + fileName << "\">" << fileName << "</a>";
        // Agrega el botón de borrado para archivos
        if (!S_ISDIR(fileStat.st_mode)) { // Solo agregar botón para archivos
            responseBody << "<button onclick=\"deleteFile('" << requestPath.substr(6) + "/" + fileName << "')\">Borrar</button>";
        }
        responseBody << "</li>\n";
    }
    // Cierra el directorio y finaliza la respuesta HTML
    closedir(dir);
    responseBody << "</ul>\n</body>\n</html>";
    // Construye la respuesta HTTP con el contenido generado
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: " << responseBody.str().size() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n"
             << responseBody.str();
    return response.str();
}
