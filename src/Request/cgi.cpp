/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:11:50 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 14:09:14 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"

void fake_sleep(int seconds) {
    clock_t start_time = clock(); // Obtiene el tiempo actual

    // Bucle que consume tiempo
    while (static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC < seconds) {
        // No hacer nada, solo esperar
    }
}

std::string Request::handle_cgi(const std::string &script_path, const std::string &query_string, const std::string &method, const std::string &body, Location location)
{
    (void)location;
    int pipe_stdout[2]; // Para capturar salida de php-cgi
    int pipe_stdin[2];  // Para enviar datos POST

    if (pipe(pipe_stdout) == -1 || pipe(pipe_stdin) == -1) {
        return location.getErrorPage(500);
    }

    pid_t pid = fork();
    if (pid < 0) {
        return location.getErrorPage(500);
    }
    else if (pid == 0) { 
        // Proceso hijo: Ejecuta php-cgi
        close(pipe_stdout[0]);
        dup2(pipe_stdout[1], STDOUT_FILENO);
        close(pipe_stdout[1]);

        close(pipe_stdin[1]);
        dup2(pipe_stdin[0], STDIN_FILENO);
        close(pipe_stdin[0]);

        std::vector<char*> env;
        Request::_setup_cgi_env(script_path, query_string, method, body, env);

        char *argv[] = { (char*)"/usr/bin/php-cgi", (char*)script_path.c_str(), NULL };
        execve(argv[0], argv, &env[0]);
        perror("execve");
        return "";
    }
    else {
        close(pipe_stdout[1]);
        close(pipe_stdin[0]);

        // Enviar datos a través del pipe
        if (method == "POST") {
            if (!body.empty()) {
                write(pipe_stdin[1], body.c_str(), body.size());
            }
        }
        close(pipe_stdin[1]);

        int status;
        int timeout = 10; // Tiempo de espera de 10 segundos
        while (timeout > 0) {
            pid_t result = waitpid(pid, &status, WNOHANG);
            if (result == pid) {
                break; // El proceso ha terminado
            }
            fake_sleep(1); // Esperar 1s
            timeout--;
        }

        if (timeout == 0) {
            kill(pid, SIGKILL); // Matar el proceso si se excede el tiempo
            waitpid(pid, &status, 0); // Esperar a que el proceso termine
            return location.getErrorPage(504); // Error por timeout
        }

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            return location.getErrorPage(502); // Error si el proceso no terminó correctamente
        }

        char buffer[1024];
        std::string cgi_output;
        ssize_t bytes_read;

        while ((bytes_read = read(pipe_stdout[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            cgi_output += buffer;
        }
        close(pipe_stdout[0]);

        if (cgi_output.empty()) {
            return location.getErrorPage(500); // Error si la salida está vacía
        }

        return Request::build_http_response_cgi(cgi_output);
    }
}

std::string Request::build_http_response_cgi(const std::string &cgi_output)
{
    std::string headers;
    std::string body;
    size_t header_end = cgi_output.find("\r\n\r\n");

    if (header_end != std::string::npos) {
        headers = cgi_output.substr(0, header_end);
        body = cgi_output.substr(header_end + 4); // Salta los "\r\n\r\n"
    } else {
        body = cgi_output; // Si no hay separación, todo es el cuerpo
    }

    // Buscar Content-Type
    std::string content_type = "text/html"; // Valor por defecto
    size_t ct_pos = headers.find("Content-Type:");
    if (ct_pos != std::string::npos) {
        size_t start = ct_pos + 13; // Longitud de "Content-Type:"
        size_t end = headers.find("\r\n", start);
        content_type = headers.substr(start, end - start);
    }

    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: " + content_type + "\r\n"
           "Content-Length: " + Request::_int_to_string(body.size()) + "\r\n"
           "\r\n" + body;
}

void Request::_setup_cgi_env(const std::string &script_path, const std::string &query_string, const std::string &method, const std::string &body, std::vector<char*>& env)
{
    env.push_back(strdup((std::string("QUERY_STRING=") + query_string).c_str()));
    env.push_back(strdup((std::string("REQUEST_METHOD=") + method).c_str()));
    env.push_back(strdup((std::string("SCRIPT_FILENAME=") + script_path).c_str()));
    env.push_back(strdup("REDIRECT_STATUS=200"));

    if (method == "POST") {
        env.push_back(strdup((std::string("CONTENT_LENGTH=") + Request::_int_to_string(body.size())).c_str()));
        
        // Manejar uploads: se asume que se recibe como multipart/form-data
        if (body.find("Content-Disposition: form-data;") != std::string::npos) {
            // Aquí deberías establecer el boundary si existe
            std::string boundary = "---------------------------"; // Asegúrate de que esto coincida con lo que el cliente envía
            env.push_back(strdup(("CONTENT_TYPE=multipart/form-data; boundary=" + boundary).c_str()));
        } else {
            env.push_back(strdup("CONTENT_TYPE=application/x-www-form-urlencoded"));
        }
    }

    env.push_back(NULL);
}