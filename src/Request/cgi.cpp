/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:11:50 by manufern          #+#    #+#             */
/*   Updated: 2025/03/11 11:03:34 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"

std::string  Request::handle_cgi(const std::string &script_path, const std::string &query_string, Location location)
{
    (void)location;
        // Extraer la ruta del script y la query string si están combinadas
    std::string actual_script_path = script_path;
    std::string actual_query_string = query_string;
    size_t pos = script_path.find('?');
    if (pos != std::string::npos) {
        actual_script_path = script_path.substr(0, pos);
        actual_query_string = script_path.substr(pos + 1);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return build_http_response("Error al crear pipe\n", "text/plain", 500);
    }
    
    pid_t pid = fork();
    if (pid < 0) {
        return build_http_response("Error en fork()\n", "text/plain", 500);
    }
    else if (pid == 0)
    { 
        // Proceso hijo
        close(pipefd[0]); // Cerrar extremo de lectura
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        // Obtener la ruta absoluta del script si es relativa
        std::string full_script_path = actual_script_path;
        if (actual_script_path.empty() || actual_script_path[0] != '/') {
            char cwd[1024] = {0};
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                full_script_path = std::string(cwd) + "/" + actual_script_path;
            }
        }

        // Leer el shebang del script para determinar el intérprete
        int script_fd = open(full_script_path.c_str(), O_RDONLY);
        if (script_fd == -1) {
            std::cerr << "Error al abrir el script: " << full_script_path
                      << " (" << strerror(errno) << ")\n";
            _exit(1);
        }
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
            
            // Eliminar espacios y saltos de línea del intérprete detectado
            std::string::iterator dst = interpreter.begin();
            for (std::string::iterator src = interpreter.begin(); src != interpreter.end(); ++src) {
                if (!isspace(*src)) {
                    *dst = *src;
                    ++dst;
                }
            }
            interpreter.erase(dst, interpreter.end());
        }

        // Asignar un intérprete predeterminado según la extensión del archivo
        if (interpreter.empty()) {
            if (full_script_path.size() >= 3 && full_script_path.substr(full_script_path.size() - 3) == ".py")
                interpreter = "/usr/bin/python3";
            else if (full_script_path.size() >= 3 && full_script_path.substr(full_script_path.size() - 3) == ".pl")
                interpreter = "/usr/bin/perl";
            else if (full_script_path.size() >= 3 && full_script_path.substr(full_script_path.size() - 3) == ".sh")
                interpreter = "/bin/bash";
            else if (full_script_path.size() >= 4 && full_script_path.substr(full_script_path.size() - 4) == ".php")
                interpreter = "/usr/bin/php";
        }
        
        // Configurar las variables de entorno para CGI
        std::vector<char*> env;
        Request::_setup_cgi_env(full_script_path, actual_query_string, env);
        
        // Ejecutar el script CGI con el intérprete adecuado
        char *argv[] = { const_cast<char*>(interpreter.c_str()),
                         const_cast<char*>(full_script_path.c_str()),
                         NULL };
        execve(argv[0], argv, env.data());

        // Si execve falla, imprimir error y salir
        std::cerr << "Error al ejecutar el script: " << strerror(errno) << "\n";
        _exit(1);
    }
    else
    { 
        // Proceso padre
        close(pipefd[1]); // Cerrar extremo de escritura
        char buffer[1024];
        std::string cgi_output;
        ssize_t bytes_read;
        
        // Leer la salida del script CGI
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            cgi_output += buffer;
        }
        close(pipefd[0]);

        // Esperar a que termine el proceso hijo
        int status;
        waitpid(pid, &status, 0);
        
        return Request::build_http_response(cgi_output, "text/html", 200);
    }
}

// ========================================
//  FUNCIÓN: setup_cgi_env
// ========================================
// Configura las variables de entorno para la ejecución de un script CGI.
//
// Se encarga de:
// 1. Establecer la variable QUERY_STRING con los parámetros de la solicitud.
// 2. Definir el método de la solicitud como "GET".
// 3. Configurar la ruta del script CGI.
// 4. Indicar un estado de redirección exitoso (REDIRECT_STATUS=200).
// 5. Almacenar las variables de entorno en un vector de punteros.
//
// Parámetros:
// - script_path: Ruta del script CGI sin la query string.
// - query_string: Cadena con los parámetros de la consulta.
// - env: Vector donde se almacenarán las variables de entorno.
//
// Retorno:
// - Ninguno.
void    Request::_setup_cgi_env(const std::string &script_path, const std::string &query_string, std::vector<char*>& env)
{
    std::string query = "QUERY_STRING=" + query_string;
    std::string method = "REQUEST_METHOD=GET";
    std::string script = "SCRIPT_FILENAME=" + script_path;
    std::string redirect = "REDIRECT_STATUS=200";
    
    env.push_back(strdup(query.c_str()));
    env.push_back(strdup(method.c_str()));
    env.push_back(strdup(script.c_str()));
    env.push_back(strdup(redirect.c_str()));
    env.push_back(NULL);  // Termina el arreglo de variables de entorno
}