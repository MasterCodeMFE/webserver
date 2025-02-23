# Reglas del fichero de configuración

0. A Partir de `#` se considera **comentario**. Para hacer comentarios multilinea, cada linea tiene que iniciarse con `#`.</br>
1. Para especificar valores **literales** se utilizan `"`.
2. `'` devolverán error si aparecen en cualquier parte del fichero de configuración que no sean un comentario. 
3. `\` no aceptado para saltar caracteres especiales. Devolverán error si aparecen en cualquier parte del fichero de </br>
    configuración que no sean un comentario. su utilización retornará error salvo que esté dentro de un comentario.
1. Puede contener varios bloques `server` con su contenido detallado entre {}. Ejemplo: serverA { ... }
2. Puede haber varias lineas de `server_name` por servidor, cada una con un nombre y se almacenarán los valores unicos. Su declaración termina con `;`. Ejemplo: server_name a;
3. `listen`, puede haber varios por servidor y su estructura será: </br>
	```
		listen  address[:port];
       	listen	port;
	```
4. `error_page` puede incluirse a nivel global, en el bloque server y en el bloque location. El rango de códigos
	es de 300 a 599, otros códigos devolverán error. Primara el error page más concreto asociado a un código- Por sentencia se podrá declarar un codigo y su página asociada. En caso de que un código se repita, la pagina asociada que prevalecerá será la última declarada. Su declaración termina con `;`.
	 Ejemplo: error_code 401 ./page40x.html;
5.	`client_max_body`  puede incluirse a nivel global, en el bloque server y en el bloque location.
	Formato = digitos[kKmMgG]. Si duplicado en el mismo bloque, actualiza. 
	Por defecto el tamaño es en bytes (k=kilobytes ...). 
	Su declaración termina con `;`. Ejemplo: client_max_body_size 1g;
6.	`location uri{ ... }` Debe estar dentro de server y puede haber varias por server. La uri puede ser
	un directorio o un fichero. Error de duplicado si en el mismo server existe la misma location.
7.	`method <metodo>;` Debe contenerse dentro del bloque `location`. Añade a la lista si no está duplicado.
	Una declaración de método por palabra clave `method`.
8.	`redirecto from to;` Debe contenerse dentro de bloque server o location. Indica la redirección de la ruta from a to.
	Si duplicado en el mismo bloque, actualiza.
9.	`alias /path` Detro del bloque location. Convierte la location al path indicado. Si duplicado en el mismo bloque, actualiza.
10.	`root path`puede incluirse a nivel global, en el bloque server y en el bloque location. 
	Indica el directorio de partida. Si duplicado en el mismo bloque, actualiza.
11.	`autoindex on|off` puede incluirse a nivel global, en el bloque server y en el bloque location. Si duplicado en el mismo bloque, actualiza.
	Indica si se listan los ficheros de un directorio cuando se pasa una ruta en vez
	de un archivo e index no esta especificado
12. `index str` puede incluirse a nivel global, en el bloque server y en el bloque location. Si duplicado en el mismo bloque, actualiza.
	 Fichero que se usa por defecto al solicitar una directiva.

Para configurar un CGI especificaremos una carpeta `location`y que cada vez que haya una interacción en esa carpeta
se ejecute un CGI indicado