Para replicar el parsing de un archivo de configuración de Nginx en C++98, puedes seguir estos pasos:

1. **Leer el archivo de configuración**: Abre el archivo y lee su contenido línea por línea.
2. **Eliminar comentarios y espacios en blanco**: Ignora las líneas que son comentarios y elimina los espacios en blanco innecesarios.
3. **Analizar las directivas y bloques**: Identifica las directivas y los bloques de configuración (por ejemplo, `server`, [`location`](command:_github.copilot.openSymbolFromReferences?%5B%7B%22%24mid%22%3A1%2C%22path%22%3A%22%2Fhome%2Fpabad-ap%2Fwebserv%2Ftemplates%2FmainTestConfig.cpp%22%2C%22scheme%22%3A%22file%22%7D%2C%7B%22line%22%3A10%2C%22character%22%3A13%7D%5D "templates/mainTestConfig.cpp")).
4. **Almacenar la configuración**: Utiliza estructuras de datos adecuadas para almacenar la configuración de manera jerárquica.

Aquí tienes un ejemplo de cómo podrías implementar esto en C++98:

### Pseudocódigo
1. Abrir el archivo de configuración.
2. Leer el archivo línea por línea.
3. Eliminar comentarios y espacios en blanco.
4. Analizar las directivas y bloques.
5. Almacenar la configuración en estructuras de datos.

# Anotacines personales a revisar
1. No server dentro de server, check de que solo haya atributos esperados dentro de cada categoría.
2. Check llave abierta, llave cerrada.
3. `Server` y `listen` -> `{}`
    `server_name`, `listen`,`error_page`, `client_max_body`, `index`, `methods`, `redirection`, `autoindex`, `cgi`, `root` -> `;`
4. Información detras de `#` en la misma linea == comentario.

```
Estructura mensajes de error: timestamp [<error_type>] <errno> # <errno>: <error description>

Posibles errores:
    - Unexpected EOF, expecting '}'
    - Directive <...> has no opening '{'
    - <...> is not terminated by ';'
```

SERVER { ... }
SERVER_NAME name ...; #default empty
LISTEN  address[:port]
        port
        unix:path #UNIX socket
ERROR_PAGE  code ...    URI;
CLIENT_MAX_BODY size; #Formato = digitos[kKmMgG]
LOCATION uri { ... }
METHODS method ...;
REDIRECT    from to;
AUTOINDEX   on|off;
CGI         str;
ROOT    path(ruta absoluta); # default ./