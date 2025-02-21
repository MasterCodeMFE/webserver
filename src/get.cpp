#include "test.hpp"

// Maneja las solicitudes GET.
std::string handle_get(const HttpRequest& request, const Config &config)
{
    (void)config; // Config no utilizado en este ejemplo
    std::string filepath = get_file_path(request.path);

    struct stat file_stat;
    if (::stat(filepath.c_str(), &file_stat) != 0)
    {
        std::cerr << "Error: No se encontró el archivo o directorio " << filepath << std::endl;
        return Status::getDefaultErrorPage(404); // Archivo no encontrado
    }
    if (S_ISDIR(file_stat.st_mode))
    {
        return listDirectory(filepath, request.path);
    }
    if (access(filepath.c_str(), R_OK) != 0)
    {
        return Status::getDefaultErrorPage(403); // Sin permisos de lectura
    }
    std::string content = read_file(filepath);
    if (content.empty())
    {
        return Status::getDefaultErrorPage(500); // Error interno
    }
    std::string content_type = get_content_type(filepath);
    return build_http_response(content, content_type, 200);
}
