#include "test.hpp"

// Maneja las solicitudes DELETE.
std::string handle_delete(const HttpRequest& httpRequest)
{
    std::string filepath = "www" + httpRequest.path;
    if (access(filepath.c_str(), F_OK) != 0)
    {
        std::string error = "Archivo no existe\n";
        return build_http_response(error, "text/plain", 404);
    }
    if (remove(filepath.c_str()) != 0)
    {
        std::string error = "Error al eliminar el archivo\n";
        return build_http_response(error, "text/plain", 500);
    }
    std::string success = "Archivo eliminado con éxito\n";
    return build_http_response(success, "text/plain", 200);
}