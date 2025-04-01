/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:07 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 20:02:34 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string  Request::handle_get(const HttpRequest& request, Location location)
{
    std::string filepath = Request::_get_file_path(request.path, location);

    struct stat file_stat;
    
    if (::stat(filepath.c_str(), &file_stat) != 0)
    {
        return (location.getErrorPage(404));
    }
    
    if (S_ISDIR(file_stat.st_mode)  && location.getAutoindex() == true)
    {
        return Request::_listDirectory(filepath, request.path, location);
    }
    else if(S_ISDIR(file_stat.st_mode)  && location.getAutoindex() == false)
    {
        return (location.getErrorPage(404));
    }
    
    if (access(filepath.c_str(), R_OK) != 0)
    {
        return Status::getErrorPage(403);
    }

    std::string content = Request::_read_file(filepath);
    std::string content_type = Request::_get_content_type(filepath);

    return build_http_response(content, content_type, 200);
}


bool containsIndexHtml(std::string directoryPath, std::string index)
{
    DIR *dir = opendir(directoryPath.c_str());
    if (!dir)
    {
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

std::string Request::_get_file_path(const std::string &request_path, Location location) {
    struct stat file_stat;
    std::string index = location.getIndex();

    if (index.empty()) {
        index = "index.html";
    }
    std::string path = request_path;
    if (::stat(path.c_str(), &file_stat) != 0) {
        return "";
    }
    if (S_ISDIR(file_stat.st_mode)) {
        if (containsIndexHtml(path, index) == true) {
            if (path[path.size() - 1] != '/') {
                path += "/";
            }
            path += index;
            return path;
        }
    }

    return request_path;
}

std::string Request::_listDirectory(const std::string &dirPath, const std::string &requestPath, Location location)
{
    if (access(dirPath.c_str(), F_OK) != 0) {
        return location.getErrorPage(404);
    }
    if (access(dirPath.c_str(), R_OK) != 0) {
        return location.getErrorPage(403);
    }
    DIR *dir = opendir(dirPath.c_str());
    if (!dir) {
        return location.getErrorPage(500);
    } 
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
        std::string fullPath = dirPath + (dirPath[dirPath.size() - 1] == '/' ? "" : "/") + fileName;
        struct stat fileStat;
        if (stat(fullPath.c_str(), &fileStat) != 0) {
            continue;
        }

        std::string icon = "📄";
        if (S_ISDIR(fileStat.st_mode)) {
            icon = "📁";
            responseBody << "<li>" << icon << " <a href=\"" << fullPath.substr(5) << "\">" << fileName << "</a>";
        }
        if (!S_ISDIR(fileStat.st_mode)) { 
            responseBody << "<li>" << icon << " <a href=\"" << requestPath.substr(location.getRoot().size()) + "/" + fileName << "\">" << fileName << "</a>";
            responseBody << "<button onclick=\"deleteFile('" << requestPath.substr(location.getRoot().size()) + "/" + fileName << "')\">Borrar</button>";
        }
        responseBody << "</li>\n";
    }
    closedir(dir);
    responseBody << "</ul>\n</body>\n</html>";
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: " << responseBody.str().size() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n"
             << responseBody.str();
    return response.str();
}
