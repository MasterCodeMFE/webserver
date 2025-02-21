#include "test.hpp"

// -----------------------------------------------------------------------------
// Manejo de archivos y directorios
// -----------------------------------------------------------------------------

// Lista el contenido de un directorio en formato HTML.
std::string listDirectory(const std::string &dirPath, const std::string &requestPath)
{
    std::cout << "Intentando listar: " << dirPath << std::endl;
    
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
        std::cerr << "Error: No se pudo abrir el directorio " << dirPath
                  << " (" << strerror(errno) << ")" << std::endl;

        return Status::getDefaultErrorPage(404);
    }

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

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string fileName = entry->d_name;
        if (fileName != "." && fileName != "..")
        {
            std::string fullPath = dirPath + "/" + fileName;
            struct stat fileStat;
            
            std::string icon = "📄"; // Icono por defecto para archivos
            if (stat(fullPath.c_str(), &fileStat) == 0 && S_ISDIR(fileStat.st_mode))
            {
                icon = "📁"; // Si es un directorio
            }
            
            responseBody << "<li>" << icon << " <a href=\"" << requestPath + "/" + fileName << "\">" << fileName << "</a></li>\n";
        }
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


// Devuelve la ruta del archivo basándose en la ruta de la solicitud.
std::string get_file_path(const std::string& request_path)
{
    if (request_path == "/")
        return "www/index.html";
    return "www" + request_path;
}