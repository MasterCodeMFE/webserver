//CABECERA

#include "Request.hpp"

static std::string save_uploaded_file(HttpRequest httpRequest, Location location);
static std::string extract_filename(const std::string& body);

std::string Request::handle_post(const HttpRequest& httpRequest, Location location)
{
    std::stringstream ss;
    unsigned long contentLength = 0;
    
    try {
        // Obtener Content-Length o Transfer-Encoding
        std::map<std::string, std::string>::const_iterator it = httpRequest.headers.find("Content-Length");
        if (it != httpRequest.headers.end()) {
            ss << it->second;
            ss >> contentLength;
        } else if ((it = httpRequest.headers.find("Transfer-Encoding")) != httpRequest.headers.end() && it->second == "chunked") {
            return location.getErrorPage(501);
        } else {
            return location.getErrorPage(400);
        }
    
        // Validar tamaño del cuerpo
        if (contentLength > location.getClienteMaxBodySize()) {
            return location.getErrorPage(413);
        }
    
        // Obtener Content-Type
        std::string content_type;
        it = httpRequest.headers.find("Content-Type");
        if (it != httpRequest.headers.end()) {
            content_type = it->second;
        } else {
            return location.getErrorPage(400);
        }
    
        std::string response_body;
    
        std::cout << "\n📥 Datos recibidos en el POST (Tamaño: " << httpRequest.body.size() << " bytes):\n"
                  << httpRequest.body << "\n";
    
        // Manejar multipart/form-data
        if (content_type.find("multipart/form-data") != std::string::npos) {
            size_t pos = content_type.find("boundary=");
            if (pos == std::string::npos) {
                return location.getErrorPage(400);
            }
    
            // El boundary ya incluye los guiones en la petición
            std::string boundary = "--" + content_type.substr(pos + 9);
            if (boundary.empty() || boundary == "--") {
                return location.getErrorPage(400);
            }
    
            response_body = save_uploaded_file(httpRequest, location);
        } else {
            response_body = "POST recibido con éxito\nDatos:\n" + httpRequest.body;
        }
    
        return build_http_response(response_body, "text/plain", 201);
    }
    catch (const std::exception& e) {
        std::cerr << "Excepción en handle_post: " << e.what() << "\n";
        return location.getErrorPage(500);
    }
}

std::string extract_filename(const std::string& body)
{
    size_t pos = body.find("filename=\"");
    if (pos == std::string::npos)
        return "archivo"; // Usar "archivo" en vez de "archivo_desconocido"

    pos += 10; // Saltar 'filename="'
    size_t end = body.find("\"", pos);
    if (end == std::string::npos)
        return "archivo";

    std::string filename = body.substr(pos, end - pos);

    // Eliminar espacios en blanco alrededor (C++98 compatible)
    while (!filename.empty() && (filename[0] == ' ' || filename[0] == '\t'))
        filename.erase(0, 1);
    while (!filename.empty() && (filename[filename.length() - 1] == ' ' || filename[filename.length() - 1] == '\t'))
        filename.erase(filename.length() - 1, 1);

    // Si el nombre está vacío después de limpiar, usar "archivo"
    if (filename.empty())
        return "archivo";

    // Si el nombre es solo un punto ".", es inválido
    if (filename == ".")
        return "archivo";

    return filename;
}

size_t str_to_size_t(const std::string& str)
{
    std::istringstream iss(str);
    size_t result;
    iss >> result;
    return result;
}

// Función auxiliar para convertir int a string en C++98
std::string int_to_string(int num)
{
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

std::string get_unique_filename(const std::string& directory, const std::string& filename)
{
    std::string base_name = filename;
    std::string extension = "";
    size_t dot_pos = filename.rfind('.');

    // Si el nombre no tiene punto, dejarlo como está
    if (dot_pos != std::string::npos && dot_pos != 0) {
        base_name = filename.substr(0, dot_pos);
        extension = filename.substr(dot_pos);
    }

    std::string new_filename = filename;
    int count = 1;

    // Verificar si el archivo ya existe
    struct stat buffer;
    while (stat((directory + new_filename).c_str(), &buffer) == 0) {
        std::ostringstream oss;
        oss << base_name << "_" << count << extension;
        new_filename = oss.str();
        count++;
    }

    return new_filename;
}

std::string save_uploaded_file(HttpRequest httpRequest, Location location)
{
    size_t content_length = 0;
    std::map<std::string, std::string>::iterator it = httpRequest.headers.find("Content-Length");
    if (it != httpRequest.headers.end()) {
        std::istringstream iss(it->second);
        iss >> content_length;
    } else {
        return location.getErrorPage(411); // Error 411 si falta Content-Length
    }

    if(location.getClienteMaxBodySize() < content_length)
    {
        return location.getErrorPage(413); // Error 413 si el archivo es demasiado grande
    }

    const std::string& body = httpRequest.body;
    std::string filename = extract_filename(body); 
    
    std::cout << "---------------ca--------------------" << std::endl << body << std::endl;
    
    std::string upload_dir = "www/upload/";
    struct stat st;
    if (stat(upload_dir.c_str(), &st) != 0) {
        if (mkdir(upload_dir.c_str(), 0777) != 0) {
            return location.getErrorPage(500); // Error 500 si falla la creación del directorio
        }
    }

    // Obtener un nombre único si el archivo ya existe
    filename = get_unique_filename(upload_dir, filename);
    
    std::string file_path = upload_dir + filename;

    std::ofstream outfile(file_path.c_str(), std::ios::binary);
    if (!outfile) {
        return location.getErrorPage(500); // Error 500 si no se puede escribir el archivo
    }
    outfile.write(body.c_str(), body.size());
    outfile.close();
    
    return "HTTP/1.1 201 Created\r\n"; // Éxito al guardar el archivo
}
