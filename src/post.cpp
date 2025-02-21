#include "test.hpp"

// Extrae el nombre de archivo del cuerpo de la solicitud POST.
std::string extract_filename(const std::string& body) {
    std::string header_start = "filename=\"";
    size_t filename_pos = body.find(header_start);
    if (filename_pos == std::string::npos)
        return "archivo_subido"; // Nombre por defecto

    filename_pos += header_start.length();
    size_t filename_end = body.find("\"", filename_pos);
    if (filename_end == std::string::npos)
        return "archivo_subido";
    return body.substr(filename_pos, filename_end - filename_pos);
}

// Guarda un archivo subido mediante POST.
std::string save_uploaded_file(const std::string& body, const std::string& boundary) {
    // Extraer el nombre del archivo
    std::string filename = extract_filename(body);
    // Buscar el inicio y fin del contenido del archivo
    size_t file_start = body.find("\r\n\r\n") + 4;
    size_t file_end = body.rfind("--" + boundary) - 2; // Excluir "--"
    if (file_start == std::string::npos || file_end == std::string::npos || file_end <= file_start) {
        return "Error: No se pudo encontrar el archivo en la solicitud.";
    }
    std::string file_content = body.substr(file_start, file_end - file_start);
    std::string file_path = "upload/" + filename;
    std::ofstream outfile(file_path.c_str(), std::ios::binary);
    if (!outfile) {
        return "Error: No se pudo guardar el archivo.";
    }
    outfile.write(file_content.c_str(), file_content.size());
    outfile.close();
    return "Archivo guardado en: " + file_path;
}

// Maneja las solicitudes POST.
std::string handle_post(const HttpRequest& httpRequest)
{
    // Verificar existencia de Content-Type
    std::map<std::string, std::string>::const_iterator it = httpRequest.headers.find("Content-Type");
    if (it == httpRequest.headers.end()) {
        std::string error = "Falta Content-Type\n";
        return build_http_response(error, "text/plain", 400);
    }
    std::string content_type = it->second;
    std::string response_body;

    std::cout << "📥 Datos recibidos en el POST:\n" << httpRequest.body << "\n";

    if (content_type.find("multipart/form-data") != std::string::npos) {
        // Extraer el boundary
        size_t pos = content_type.find("boundary=");
        if (pos == std::string::npos) {
            std::string error = "Falta el boundary en Content-Type\n";
            return build_http_response(error, "text/plain", 400);
        }
        std::string boundary = "--" + content_type.substr(pos + 9);
        // Verificar si se encontró un archivo
        std::string filename;
        size_t filename_pos = httpRequest.body.find("filename=\"");
        if (filename_pos != std::string::npos) {
            filename_pos += 10;
            size_t filename_end = httpRequest.body.find("\"", filename_pos);
            if (filename_end != std::string::npos)
                filename = httpRequest.body.substr(filename_pos, filename_end - filename_pos);
        }
        if (!filename.empty()) {
            response_body = save_uploaded_file(httpRequest.body, boundary);
        } else {
            std::string error = "No se detectó un archivo adjunto\n";
            return build_http_response(error, "text/plain", 400);
        }
    } else {
        response_body = "POST recibido con éxito\nDatos:\n" + httpRequest.body;
    }
    return build_http_response(response_body, "text/plain", 200);
}