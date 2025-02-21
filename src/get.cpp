#include "test.hpp"

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

std::string handle_get(const HttpRequest& request, const Config &config)
{
    (void)config; // Config no utilizado en este ejemplo
    
    // Obtiene la ruta del archivo solicitado
    std::string filepath = get_file_path(request.path);

    struct stat file_stat;
    
    // Verifica si el archivo o directorio existe
    if (::stat(filepath.c_str(), &file_stat) != 0)
    {
        std::cerr << "Error: No se encontró el archivo o directorio " << filepath << std::endl;
        return Status::getDefaultErrorPage(404); // Retorna error 404 si no existe
    }
    
    // Si es un directorio, genera un listado de su contenido
    if (S_ISDIR(file_stat.st_mode))
    {
        return listDirectory(filepath, request.path);
    }
    
    // Verifica si el archivo tiene permisos de lectura
    if (access(filepath.c_str(), R_OK) != 0)
    {
        return Status::getDefaultErrorPage(403); // Retorna error 403 si no hay permisos
    }

    // Lee el contenido del archivo
    std::string content = read_file(filepath);
    if (content.empty())
    {
        return Status::getDefaultErrorPage(500); // Retorna error 500 si hay un problema interno
    }

    // Obtiene el tipo de contenido (MIME type) del archivo
    std::string content_type = get_content_type(filepath);

    // Construye y retorna la respuesta HTTP con el archivo solicitado
    return build_http_response(content, content_type, 200);
}







