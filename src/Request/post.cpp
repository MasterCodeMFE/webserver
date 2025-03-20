//CABECERA

#include "Request.hpp"

static std::string save_uploaded_file(const std::string& body, const std::string& boundary);
static std::string extract_filename(const std::string& body);

std::string Request::handle_post(const HttpRequest& httpRequest, Location location)
{
    std::stringstream ss;
    unsigned long contentLength = 0;


    std::map<std::string, std::string>::const_iterator it = httpRequest.headers.find("Content-Length");
    if (it != httpRequest.headers.end()) {
        ss << it->second;
        ss >> contentLength;
    } 
    else if ((it = httpRequest.headers.find("Transfer-Encoding")) != httpRequest.headers.end() && it->second == "chunked") {
        return build_http_response("Error: Transfer-Encoding chunked no soportado\n", "text/plain", 501);
    } 
    else {
        return build_http_response("Error: Falta Content-Length\n", "text/plain", 400);
    }

    if (contentLength > location.getClienteMaxBodySize()) {
        return location.getErrorPage(413);
    }

    it = httpRequest.headers.find("Content-Type");
    if (it == httpRequest.headers.end()) {
        return build_http_response("Error: Falta Content-Type\n", "text/plain", 400);
    }

    std::string content_type = it->second;
    std::string response_body;

    std::cout << "\n📥 Datos recibidos en el POST:\n" << httpRequest.body << "\n";

    if (content_type.find("multipart/form-data") != std::string::npos) {
        size_t pos = content_type.find("boundary=");
        if (pos == std::string::npos) {
            return build_http_response("Error: Falta boundary en Content-Type\n", "text/plain", 400);
        }

        std::string boundary = "--" + content_type.substr(pos + 9);
        response_body = save_uploaded_file(httpRequest.body, boundary);
    } 
    else {
        response_body = "POST recibido con éxito\nDatos:\n" + httpRequest.body;
    }

    return build_http_response(response_body, "text/plain", 200);
}

static std::string save_uploaded_file(const std::string& body, const std::string& boundary)
{
    std::string filename = extract_filename(body);
    size_t file_start = body.find("\r\n\r\n");
    if (file_start == std::string::npos) {
        return "Error: No se encontró el contenido del archivo.";
    }
    file_start += 4;
    size_t file_end = body.find("--" + boundary, file_start);
    if (file_end == std::string::npos) {
        return "Error: No se encontró el final del archivo.";
    }
    file_end -= 2;
    std::string file_content = body.substr(file_start, file_end - file_start);
    std::string file_path = "upload/" + filename;

    std::ofstream outfile(file_path.c_str(), std::ios::binary);
    if (!outfile) {
        return "Error: No se pudo crear el archivo.";
    }
    outfile.write(file_content.c_str(), file_content.size());
    outfile.close();

    return "Archivo guardado en: " + file_path;
}

static std::string extract_filename(const std::string& body)
{
    std::string header_start = "filename=\"";
    size_t filename_pos = body.find(header_start);
    if (filename_pos == std::string::npos)
        return "archivo_subido";

    filename_pos += header_start.length();
    size_t filename_end = body.find("\"", filename_pos);
    if (filename_end == std::string::npos)
        return "archivo_subido";
    return body.substr(filename_pos, filename_end - filename_pos);
}

