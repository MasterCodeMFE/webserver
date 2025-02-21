#include "test.hpp"

// -----------------------------------------------------------------------------
// Manejo de archivos y directorios
// -----------------------------------------------------------------------------

// Lista el contenido de un directorio en formato HTML.
std::string listDirectory(const std::string &dirPath)
{
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
        return "<html><body><h1>Error: No se pudo abrir el directorio</h1></body></html>";

    std::string response = "<html><body><h1>Índice de " + dirPath + "</h1><ul>";
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string fileName = entry->d_name;
        // Omitir "." y ".."
        if (fileName != "." && fileName != "..")
        {
            response += "<li><a href=\"" + fileName + "\">" + fileName + "</a></li>";
        }
    }
    closedir(dir);
    response += "</ul></body></html>";
    return response;
}

// Devuelve la ruta del archivo basándose en la ruta de la solicitud.
std::string get_file_path(const std::string& request_path)
{
    if (request_path == "/")
        return "www/index.html";
    return "www" + request_path;
}