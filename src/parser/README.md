# Reglas del fichero de configuración

0. A Partir de `#` se considera **comentario**. Para hacer comentarios multilinea, cada linea tiene que iniciarse con `#`.
1. `"` devolverán error si aparecen en cualquier parte del fichero de configuración que no sean un comentario. No se aceptan valores literales con espacio.
2. `'` devolverán error si aparecen en cualquier parte del fichero de configuración que no sean un comentario. 
3. `\` no aceptado para saltar caracteres especiales. Devolverán error si aparecen en cualquier parte del fichero de </br>
    configuración que no sean un comentario. Su utilización retornará error salvo que esté dentro de un comentario.
4. No se pueden definir directivas globales, todas las directivas deben incluirse dentro de un bloque `server`.
5. Los delimitadores de apertura de bloque (`{`)  y de directivas ( `;`) tienen que estar precedidos por algun tipo de espacio. Ejemplo: server{...} dara error, server { ... } sera correcto.
6. Puede contener varios bloques `server` con su contenido detallado entre {}. Ejemplo: serverA { ... }
7. Puede haber un o ningun `server_name` por servidor, en cualquier otro caso retornará una excepción. Su declaración termina con ` ;`. Ejemplo: server_name a ;
8. `listen`, uno por servidor y su estructura será `listen  address:port ;`.
9. `error_page` puede incluirse en el bloque server y en el bloque location. El rango de códigos
	es de 300 a 599, otros códigos devolverán error. Primara el error page más concreto asociado a un código- Por sentencia se podrá declarar un codigo y su página asociada. En caso de que un código se repita, la pagina asociada que prevalecerá será la última declarada. Su declaración termina con `;`.
	 Ejemplo: error_code 401 ./page40x.html;
10.	`client_max_body_size`  puede incluirse en el bloque server y en el bloque location.
	Formato = digitos[kKmMgG]. Si duplicado en el mismo bloque, actualiza. 
	Por defecto el tamaño es en bytes (k=kilobytes ...). 
	Su declaración termina con `;`. Ejemplo: client_max_body_size 1g;
11.	`location uri { ... }` Debe estar dentro de server y puede haber varias por server. La uri puede ser
	un directorio o un fichero. Error de duplicado si en el mismo server existe la misma location.
12.	`method <metodo>;` Debe contenerse dentro del bloque `location`. Añade a la lista si no está duplicado.
	Una declaración de método por palabra clave `method`.
13.	`redirect from to;` Debe contenerse dentro de bloque server o location. Indica la redirección de la ruta from a to.
	Si duplicado en el mismo bloque, actualiza.
14.	`alias /path` Dentro del bloque location. Convierte la location al path indicado. Si duplicado en el mismo bloque, actualiza. **En caso de declararse alias y root en location
prevalecerá la especificación alias.**
15.	`root path`puede incluirse en el bloque server y en el bloque location. 
	Indica el directorio de partida. Si duplicado en el mismo bloque, actualiza.
16.	`autoindex on|off` puede incluirse en el bloque server y en el bloque location. Si duplicado en el mismo bloque, actualiza.
	Indica si se listan los ficheros de un directorio cuando se pasa una ruta en vez de un archivo e index no esta especificado
17. `index str` puede incluirse  en el bloque server y en el bloque location. Si duplicado en el mismo bloque, actualiza.
	 Fichero que se usa por defecto al solicitar una directiva.

Para configurar un CGI especificaremos una carpeta `location`y que cada vez que haya una interacción en esa carpeta se ejecute un CGI indicado.

### Ambito de declaración por directiva

| KEYWORD              | GLOBAL | SERVER | LOCATION |
|----------------------|:------:|:------:|:--------:|
| server               |   X    |        |          |
| server_name          |        |   X    |          |
| listen               |        |   X    |          |
| error_page           |        |   X    |    X     |
| client_max_body_size |        |   X    |    X     |
| location             |        |   X    |          |
| method               |        |        |    X     |
| redirect             |        |   X    |    X     |
| alias                |        |        |    X     |
| root                 |        |   X    |    X     |
| autoindex            |        |   X    |    X     |
| index                |        |   X    |    X     |
