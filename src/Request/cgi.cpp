/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:11:50 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 20:09:43 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"




void fake_sleep(int seconds) {
    clock_t start_time = clock();
    while (static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC < seconds) {
    }
}

std::string Request:: handle_cgi(const std::string &script_path, std::string &query_string, const std::string &method, const std::string &body, Location location)   {
    int pipe_stdout[2];
    int pipe_stdin[2];

    size_t pos = script_path.find('?');
    std::string clean_script_path = script_path;

    if (method == "GET") {
        query_string.clear();
        if (pos != std::string::npos) {
            query_string = script_path.substr(pos + 1);
            clean_script_path = script_path.substr(0, pos);
        }
    } else {
        clean_script_path = script_path;
    }

    struct stat file_stat;
    if (stat(clean_script_path.c_str(), &file_stat) != 0 || 
        !S_ISREG(file_stat.st_mode) || 
        access(clean_script_path.c_str(), X_OK) != 0) {
        return location.getErrorPage(404);
    }

    if (pipe(pipe_stdout) == -1 || pipe(pipe_stdin) == -1) {
        return location.getErrorPage(500);
    }

    pid_t pid = fork();
    if (pid < 0) {
        close(pipe_stdout[0]);
        close(pipe_stdout[1]);
        close(pipe_stdin[0]);
        close(pipe_stdin[1]);
        return location.getErrorPage(500);
    } 
    else if (pid == 0) {
        close(pipe_stdout[0]);
        dup2(pipe_stdout[1], STDOUT_FILENO);
        close(pipe_stdout[1]);

        close(pipe_stdin[1]);
        if (method == "POST") {
            dup2(pipe_stdin[0], STDIN_FILENO);
        }
        close(pipe_stdin[0]);

        std::vector<char*> env;
        _setup_cgi_env(clean_script_path, query_string, method, body, env);

        std::string interpreters[] = {"/usr/bin/php-cgi", "/usr/local/bin/php-cgi"};
        bool found = false;
        std::string php_cgi_path;
        
        for (size_t i = 0; i < sizeof(interpreters)/sizeof(interpreters[0]); ++i) {
            if (access(interpreters[i].c_str(), X_OK) == 0) {
                php_cgi_path = interpreters[i];
                found = true;
                break;
            }
        }
        
        if (!found) {
            return "";
        }

        char *argv[] = { const_cast<char*>(php_cgi_path.c_str()), 
                         const_cast<char*>(clean_script_path.c_str()), NULL };
        execve(argv[0], argv, &env[0]);
        
        return ""; 
    } 
    else { 
        close(pipe_stdout[1]);
        close(pipe_stdin[0]);

        if (method == "POST" && !body.empty()) {
            write(pipe_stdin[1], body.c_str(), body.size());
        }
        close(pipe_stdin[1]);


        int status;
        int timeout = 10;
        while (timeout > 0) {
            pid_t result = waitpid(pid, &status, WNOHANG);
            if (result == pid) {
                break;
            }
            fake_sleep(1);
            timeout--;
        }

        if (timeout == 0) {
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            close(pipe_stdout[0]);
            return location.getErrorPage(504);
        }

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            close(pipe_stdout[0]);
            return location.getErrorPage(502);
        }


        std::string cgi_output;
        char buffer[1024];
        ssize_t bytes_read;
        
        while ((bytes_read = read(pipe_stdout[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            cgi_output += buffer;
        }
        close(pipe_stdout[0]);

        if (cgi_output.empty()) {
            return location.getErrorPage(500);
        }

        return build_http_response_cgi(cgi_output);
    }
}

void Request::_setup_cgi_env(const std::string &script_path, const std::string &query_string, 
                           const std::string &method, const std::string &body, 
                           std::vector<char*>& env) {
    env.clear();

    env.push_back(strdup("REDIRECT_STATUS=200"));
    env.push_back(strdup(("SCRIPT_FILENAME=" + script_path).c_str()));
    env.push_back(strdup(("REQUEST_METHOD=" + method).c_str()));
    
    if (method == "GET" && !query_string.empty()) {
        env.push_back(strdup(("QUERY_STRING=" + query_string).c_str()));
    }
    else if (method == "POST") {
        std::ostringstream oss;
        oss << body.length();
        env.push_back(strdup(("CONTENT_LENGTH=" + oss.str()).c_str()));
        env.push_back(strdup("CONTENT_TYPE=application/x-www-form-urlencoded"));
    }

    env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
    env.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
    env.push_back(strdup("PATH_INFO="));
    
    size_t last_slash = script_path.find_last_of('/');
    std::string script_name = (last_slash != std::string::npos) ? 
                             script_path.substr(last_slash + 1) : script_path;
    env.push_back(strdup(("SCRIPT_NAME=" + script_name).c_str()));
    
    env.push_back(NULL);
}

std::string Request::build_http_response_cgi(const std::string &cgi_output) {
    size_t header_end = cgi_output.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        return "HTTP/1.1 500 Internal Server Error\r\n"
               "Content-Type: text/html\r\n"
               "Content-Length: 0\r\n\r\n";
    }

    std::string headers = cgi_output.substr(0, header_end);
    std::string body = cgi_output.substr(header_end + 4);

    std::string content_type = "text/html";
    std::string status = "200 OK";

    size_t ct_pos = headers.find("Content-Type:");
    if (ct_pos != std::string::npos) {
        size_t start = ct_pos + 13;
        while (start < headers.size() && isspace(headers[start])) start++;
        size_t end = headers.find("\r\n", start);
        content_type = headers.substr(start, end - start);
    }

    size_t status_pos = headers.find("Status:");
    if (status_pos != std::string::npos) {
        size_t start = status_pos + 7;
        while (start < headers.size() && isspace(headers[start])) start++;
        size_t end = headers.find("\r\n", start);
        status = headers.substr(start, end - start);
    }

    std::ostringstream response;
    response << "HTTP/1.1 " << status << "\r\n"
             << "Content-Type: " << content_type << "\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n" << body;

    return response.str();
}