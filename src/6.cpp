#include "test.hpp"


// -----------------------------------------------------------------------------
// Funciones auxiliares
// -----------------------------------------------------------------------------

// Lee un archivo y devuelve su contenido.
std::string read_file(const std::string& filepath) {
    std::ifstream file(filepath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Convierte un entero a std::string (compatible con C++98).
std::string int_to_string(int number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
}

// Devuelve el tipo de contenido basado en la extensión del archivo.
std::string get_content_type(const std::string& filepath) {
    std::map<std::string, std::string> mime_types;
    mime_types[".html"] = "text/html";
    mime_types[".css"] = "text/css";
    mime_types[".js"] = "application/javascript";
    mime_types[".png"] = "image/png";
    mime_types[".jpg"] = "image/jpeg";
    mime_types[".jpeg"] = "image/jpeg";
    mime_types[".gif"] = "image/gif";
    mime_types[".txt"] = "text/plain";

    size_t dot_pos = filepath.find_last_of(".");
    if (dot_pos != std::string::npos) {
        std::string ext = filepath.substr(dot_pos);
        if (mime_types.find(ext) != mime_types.end()) {
            return mime_types[ext];
        }
    }
    return "application/octet-stream"; // Tipo por defecto
}

// Construye una respuesta HTTP a partir del contenido, tipo y código de estado.
std::string build_http_response(const std::string& content, const std::string& content_type, int status_code = 200) {
    std::string status_text;
    switch (status_code) {
        case 200: status_text = "OK"; break;
        case 400: status_text = "Bad Request"; break;
        case 403: status_text = "Forbidden"; break;
        case 404: status_text = "Not Found"; break;
        case 405: status_text = "Method Not Allowed"; break;
        case 500: status_text = "Internal Server Error"; break;
        default: status_text = "OK"; break;
    }
    return "HTTP/1.1 " + int_to_string(status_code) + " " + status_text + "\r\n"
           "Content-Type: " + content_type + "\r\n"
           "Content-Length: " + int_to_string(content.size()) + "\r\n\r\n" + content;
}

// Envía todos los datos a través del socket.
ssize_t send_all(int sockfd, const char* buffer, size_t length) {
    size_t total_sent = 0;
    while (total_sent < length) {
        ssize_t bytes_sent = send(sockfd, buffer + total_sent, length - total_sent, 0);
        if (bytes_sent <= 0) {
            std::cerr << "❌ Error enviando datos al cliente." << std::endl;
            return -1;
        }
        total_sent += bytes_sent;
    }
    return total_sent;
}

// -----------------------------------------------------------------------------
// Manejo de archivos y directorios
// -----------------------------------------------------------------------------

// Lista el contenido de un directorio en formato HTML.
std::string listDirectory(const std::string &dirPath) {
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
        return "<html><body><h1>Error: No se pudo abrir el directorio</h1></body></html>";

    std::string response = "<html><body><h1>Índice de " + dirPath + "</h1><ul>";
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string fileName = entry->d_name;
        // Omitir "." y ".."
        if (fileName != "." && fileName != "..") {
            response += "<li><a href=\"" + fileName + "\">" + fileName + "</a></li>";
        }
    }
    closedir(dir);
    response += "</ul></body></html>";
    return response;
}

// Devuelve la ruta del archivo basándose en la ruta de la solicitud.
std::string get_file_path(const std::string& request_path) {
    if (request_path == "/")
        return "www/index.html";
    return "www" + request_path;
}

// -----------------------------------------------------------------------------
// Manejo de solicitudes HTTP
// -----------------------------------------------------------------------------

// Maneja las solicitudes GET.
std::string handle_get(const HttpRequest& request, const Config &config) {
    (void)config; // Config no utilizado en este ejemplo
    std::string filepath = get_file_path(request.path);

    struct stat file_stat;
    if (::stat(filepath.c_str(), &file_stat) != 0) {
        return Status::getDefaultErrorPage(404); // Archivo no encontrado
    }
    if (S_ISDIR(file_stat.st_mode)) {
        return listDirectory(filepath);
    }
    if (access(filepath.c_str(), R_OK) != 0) {
        return Status::getDefaultErrorPage(403); // Sin permisos de lectura
    }

    std::string content = read_file(filepath);
    if (content.empty()) {
        return Status::getDefaultErrorPage(500); // Error interno
    }
    std::string content_type = get_content_type(filepath);
    return build_http_response(content, content_type, 200);
}

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
std::string handle_post(const HttpRequest& httpRequest) {
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

// Maneja las solicitudes DELETE.
std::string handle_delete(const HttpRequest& httpRequest) {
    std::string filepath = "www" + httpRequest.path;
    if (access(filepath.c_str(), F_OK) != 0) {
        std::string error = "Archivo no existe\n";
        return build_http_response(error, "text/plain", 404);
    }
    if (remove(filepath.c_str()) != 0) {
        std::string error = "Error al eliminar el archivo\n";
        return build_http_response(error, "text/plain", 500);
    }
    std::string success = "Archivo eliminado con éxito\n";
    return build_http_response(success, "text/plain", 200);
}

// -----------------------------------------------------------------------------
// Manejo de CGI
// -----------------------------------------------------------------------------

// Configura las variables de entorno para el proceso CGI.
void setup_cgi_env(const std::string &script_path, const std::string &query_string, std::vector<char*>& env) {
    std::string query = "QUERY_STRING=" + query_string;
    std::string method = "REQUEST_METHOD=GET";
    std::string script = "SCRIPT_FILENAME=" + script_path;
    std::string redirect = "REDIRECT_STATUS=200";
    env.push_back(strdup(query.c_str()));
    env.push_back(strdup(method.c_str()));
    env.push_back(strdup(script.c_str()));
    env.push_back(strdup(redirect.c_str()));
    env.push_back(NULL);
}

// Maneja la ejecución de scripts CGI.
std::string handle_cgi(const std::string &script_path, const std::string &query_string) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return build_http_response("Error al crear pipe\n", "text/plain", 500);
    }
    pid_t pid = fork();
    if (pid < 0) {
        return build_http_response("Error en fork()\n", "text/plain", 500);
    }
    else if (pid == 0) { // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        int script_fd = open(script_path.c_str(), O_RDONLY);
        if (script_fd == -1)
            exit(1);

        char shebang[256] = {0};
        ssize_t bytes_read = read(script_fd, shebang, sizeof(shebang) - 1);
        close(script_fd);

        std::string interpreter;
        if (bytes_read > 2 && shebang[0] == '#' && shebang[1] == '!') {
            shebang[bytes_read] = '\0';
            std::string first_line(shebang);
            size_t end = first_line.find('\n');
            if (end != std::string::npos)
                interpreter = first_line.substr(2, end - 2);
        }
        if (interpreter.empty()) {
            if (script_path.size() >= 3 && script_path.substr(script_path.size() - 3) == ".py")
                interpreter = "/usr/bin/python3";
            else if (script_path.size() >= 3 && script_path.substr(script_path.size() - 3) == ".pl")
                interpreter = "/usr/bin/perl";
            else if (script_path.size() >= 3 && script_path.substr(script_path.size() - 3) == ".sh")
                interpreter = "/bin/bash";
            else if (script_path.size() >= 4 && script_path.substr(script_path.size() - 4) == ".php")
                interpreter = "/usr/bin/php";
        }
        std::vector<char*> env;
        setup_cgi_env(script_path, query_string, env);
        char *argv[] = {const_cast<char*>(interpreter.c_str()), const_cast<char*>(script_path.c_str()), NULL};
        execve(argv[0], argv, env.data());
        exit(1); // En caso de fallo en execve
    }
    else { // Proceso padre
        close(pipefd[1]);
        char buffer[1024];
        std::string cgi_output;
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer)-1)) > 0) {
            buffer[bytes_read] = '\0';
            cgi_output += buffer;
        }
        close(pipefd[0]);
        int status;
        waitpid(pid, &status, 0);
        return build_http_response(cgi_output, "text/html", 200);
    }
}

// -----------------------------------------------------------------------------
// Función principal para gestionar la solicitud (paso_seis)
// -----------------------------------------------------------------------------

int paso_seis(int client_fd, const HttpRequest& httpRequest, const Config &config) {
    std::string response;
    if (httpRequest.path.find("/cgi-bin/") == 0) {
        response = handle_cgi("." + httpRequest.path, httpRequest.query_string);
    }
    else if (httpRequest.method == "GET") {
        response = handle_get(httpRequest, config);
    }
    else if (httpRequest.method == "POST") {
        response = handle_post(httpRequest);
    }
    else if (httpRequest.method == "DELETE") {
        response = handle_delete(httpRequest);
    }
    else {
        std::string error = "Metodo no permitido\n";
        response = build_http_response(error, "text/plain", 405);
    }

    std::cout << "\U0001F4E4 Enviando respuesta...\n";
    send_all(client_fd, response.c_str(), response.size());
    return 0;
}
