/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:07 by manufern          #+#    #+#             */
/*   Updated: 2025/03/06 19:12:08 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// ========================================
//  FUNCIÓN: handle_get
// ========================================
// Maneja las solicitudes HTTP GET.
//
// Se encarga de:
// 1. Obtener la ruta del archivo solicitado.
// 2. Verificar si el archivo o directorio existe.
// 3. Si es un directorio, listar su contenido.
// 4. Si no hay permisos de lectura, devolver un error 403.
// 5. Leer el contenido del archivo y generar una respuesta HTTP.
//
// Parámetros:
// - request: Objeto HttpRequest que contiene la información de la solicitud.
// - config: Configuración del servidor (no se usa en este ejemplo).
//
// Retorno:
// - Una cadena con la respuesta HTTP correspondiente.
std::string  Request::handle_get(const HttpRequest& request )
{    
    // Obtiene la ruta del archivo solicitado
    std::string filepath = Request::_get_file_path(request.path);

    struct stat file_stat;
    
    // Verifica si el archivo o directorio existe
    if (::stat(filepath.c_str(), &file_stat) != 0)
    {
        std::cerr << "Error: No se encontró el archivo o directorio " << filepath << std::endl;
        return Status::getErrorPage(404); // Retorna error 404 si no existe
    }
    
    // Si es un directorio, genera un listado de su contenido
    if (S_ISDIR(file_stat.st_mode))
    {
        return Request::_listDirectory(filepath, request.path);
    }
    
    // Verifica si el archivo tiene permisos de lectura
    if (access(filepath.c_str(), R_OK) != 0)
    {
        return Status::getErrorPage(403); // Retorna error 403 si no hay permisos
    }

    // Lee el contenido del archivo
    std::string content = Request::_read_file(filepath);
    if (content.empty())
    {
        return Status::getErrorPage(500); // Retorna error 500 si hay un problema interno
    }

    // Obtiene el tipo de contenido (MIME type) del archivo
    std::string content_type = Request::_get_content_type(filepath);

    // Construye y retorna la respuesta HTTP con el archivo solicitado
    return build_http_response(content, content_type, 200);
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
std::string Request::_get_file_path(const std::string& request_path)
{
    if (request_path == "/")
        return "www/index.html"; // Retorna el archivo index por defecto
    return "www" + request_path; // Construye la ruta del archivo en la carpeta "www"
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
std::string Request::_listDirectory(const std::string &dirPath, const std::string &requestPath)
{
    std::cout << "Intentando listar: " << dirPath << std::endl;
    
    // Intenta abrir el directorio
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
        std::cerr << "Error: No se pudo abrir el directorio " << dirPath
                  << " (" << strerror(errno) << ")" << std::endl;

        return Status::getErrorPage(404); // Retorna una página de error 404 si el directorio no existe
    }

    // Construye la respuesta HTML con el índice del directorio
    std::stringstream responseBody;
    responseBody << "<!DOCTYPE html>\n"
                 << "<html>\n"
                 << "<head>\n"
                 << "<meta charset=\"UTF-8\">\n"
                 << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                 << "<title>Índice de " << dirPath << "</title>\n"
                 << "<style>\n"
                 << "body { font-family: Arial, sans-serif; padding: 20px; background-color: #f4f4f4; }\n"
                 << "h1 { text-align: center; color: #333; }\n"
                 << "ul { list-style-type: none; padding: 0; max-width: 600px; margin: 20px auto; }\n"
                 << "li { background: white; margin: 10px 0; padding: 10px; border-radius: 5px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }\n"
                 << "a { text-decoration: none; color: #007bff; font-weight: bold; display: block; }\n"
                 << "a:hover { color: #0056b3; }\n"
                 << "</style>\n"
                 << "</head>\n"
                 << "<body>\n"
                 << "<h1>Índice de " << dirPath << "</h1>\n"
                 << "<ul>\n";

    // Recorre el contenido del directorio
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string fileName = entry->d_name;
        if (fileName != "." && fileName != "..")
        {
            std::string fullPath = dirPath + "/" + fileName;
            struct stat fileStat;
            
            // Determina si es un archivo o un directorio y asigna un icono adecuado
            std::string icon = "📄"; // Icono por defecto para archivos
            if (stat(fullPath.c_str(), &fileStat) == 0 && S_ISDIR(fileStat.st_mode))
            {
                icon = "📁"; // Si es un directorio
            }
            
            // Agrega el archivo/directorio a la lista en HTML
            responseBody << "<li>" << icon << " <a href=\"" << requestPath + "/" + fileName << "\">" << fileName << "</a></li>\n";
        }
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