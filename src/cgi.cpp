#include "test.hpp"

// -----------------------------------------------------------------------------
// Manejo de CGI
// -----------------------------------------------------------------------------

// Configura las variables de entorno para el proceso CGI.
// Se reciben la ruta del script (ya sin query string) y la query string.
void setup_cgi_env(const std::string &script_path, const std::string &query_string, std::vector<char*>& env)
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

// Maneja la ejecución de scripts CGI.
// La función ahora soporta rutas complejas en las que la query string puede estar adjunta a la ruta del script.
std::string handle_cgi(const std::string &script_path, const std::string &query_string)
{
    // Separamos la ruta real del script y la query string si la ruta contiene un '?'
    std::string actual_script_path = script_path;
    std::string actual_query_string = query_string;
    size_t pos = script_path.find('?');
    if (pos != std::string::npos) {
        actual_script_path = script_path.substr(0, pos);
        actual_query_string = script_path.substr(pos+1);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        return build_http_response("Error al crear pipe\n", "text/plain", 500);
    }
    
    pid_t pid = fork();
    if (pid < 0)
    {
        return build_http_response("Error en fork()\n", "text/plain", 500);
    }
    else if (pid == 0)
    { // Proceso hijo
        close(pipefd[0]); // Cerrar extremo de lectura
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        // Si la ruta es relativa, formar la ruta absoluta usando el directorio actual.
        std::string full_script_path = actual_script_path;
        if (actual_script_path.empty() || actual_script_path[0] != '/')
        {
            char cwd[1024] = {0};
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                full_script_path = std::string(cwd) + "/" + actual_script_path;
            }
        }

        // Abrir el archivo del script para intentar leer el shebang.
        int script_fd = open(full_script_path.c_str(), O_RDONLY);
        if (script_fd == -1)
        {
            std::cerr << "Error al abrir el script: " << full_script_path
                      << " (" << strerror(errno) << ")\n";
            _exit(1);
        }
        char shebang[256] = {0};
        ssize_t bytes_read = read(script_fd, shebang, sizeof(shebang) - 1);
        close(script_fd);

        std::string interpreter;
        if (bytes_read > 2 && shebang[0] == '#' && shebang[1] == '!')
        {
            shebang[bytes_read] = '\0';
            std::string first_line(shebang);
            size_t end = first_line.find('\n');
            if (end != std::string::npos)
                interpreter = first_line.substr(2, end - 2);
            // Eliminar manualmente espacios y saltos de línea (compatible con C++98)
            std::string::iterator dst = interpreter.begin();
            for (std::string::iterator src = interpreter.begin(); src != interpreter.end(); ++src)
            {
                if (!isspace(*src))
                {
                    *dst = *src;
                    ++dst;
                }
            }
            interpreter.erase(dst, interpreter.end());
        }
        if (interpreter.empty())
        {
            if (full_script_path.size() >= 3 && full_script_path.substr(full_script_path.size() - 3) == ".py")
                interpreter = "/usr/bin/python3";
            else if (full_script_path.size() >= 3 && full_script_path.substr(full_script_path.size() - 3) == ".pl")
                interpreter = "/usr/bin/perl";
            else if (full_script_path.size() >= 3 && full_script_path.substr(full_script_path.size() - 3) == ".sh")
                interpreter = "/bin/bash";
            else if (full_script_path.size() >= 4 && full_script_path.substr(full_script_path.size() - 4) == ".php")
                interpreter = "/usr/bin/php";
        }
        
        std::vector<char*> env;
        setup_cgi_env(full_script_path, actual_query_string, env);
        char *argv[] = { const_cast<char*>(interpreter.c_str()),
                         const_cast<char*>(full_script_path.c_str()),
                         NULL };
        execve(argv[0], argv, env.data());
        // Si execve falla, imprimir error y salir.
        std::cerr << "Error al ejecutar el script: " << strerror(errno) << "\n";
        _exit(1);
    }
    else
    { // Proceso padre
        close(pipefd[1]); // Cerrar extremo de escritura
        char buffer[1024];
        std::string cgi_output;
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer)-1)) > 0)
        {
            buffer[bytes_read] = '\0';
            cgi_output += buffer;
        }
        close(pipefd[0]);
        int status;
        waitpid(pid, &status, 0);
        return build_http_response(cgi_output, "text/html", 200);
    }
}