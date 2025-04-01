```
SERVER { ... }

SERVER_NAME name; #default empty -> Es la cabecera HOST

LISTEN  address[:port]
        port
        unix:path #UNIX socket

ERROR_PAGE  code URI;

CLIENT_MAX_BODY_SIZE size; #Formato = digitos[kKmMgG], si está más de una vez en el mismo bloque "client_max_body_size" directive is duplicate

LOCATION uri { ... }

METHODS method ...;

REDIRECT    from to;

AUTOINDEX   on|off;

INDEX   str;

CGI         str;

ROOT    path(ruta absoluta); # default ./
```

Los nombres de los métodos son sensibles a mayúsculas. Esto contrasta con los nombres de los campos de las cabeceras HTTP, que no son sensibles a mayúsculas.