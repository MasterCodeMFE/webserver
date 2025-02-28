#include "test.hpp"

// -----------------------------------------------------------------------------
// Manejo de archivos y directorios
// -----------------------------------------------------------------------------

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

std::string listDirectory(const std::string &dirPath, const std::string &requestPath)
{
    std::cout << "Intentando listar: " << dirPath << std::endl;
    
    // Intenta abrir el directorio
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
        std::cerr << "Error: No se pudo abrir el directorio " << dirPath
                  << " (" << strerror(errno) << ")" << std::endl;

        return Status::getDefaultErrorPage(404); // Retorna una página de error 404 si el directorio no existe
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

std::string get_file_path(const std::string& request_path)
{
    if (request_path == "/")
        return "www/index.html"; // Retorna el archivo index por defecto
    return "www" + request_path; // Construye la ruta del archivo en la carpeta "www"
}