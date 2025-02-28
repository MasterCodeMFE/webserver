01. Fichero inexistente ✅ - `./webserv noexist`
02. Fichero vacio ✅ - `./webserv /dev/null`
03. Fichero sin accesos de lectura, escritura, ejecución o ningun permiso ✅ - `chmod 000 tests/config_file/03.SinPermisos.conf && ./webserv tests/config_file/03.SinPermisos.conf; chmod 700 tests/config_file/03.SinPermisos.conf`.
04. Fichero con directiva fuera de zona ✅
05. No poner cierre de sentencia, `;` para tipo E_DIRECTIVE o `}` para tipo E_BLOCK. ✅
06. Caracteres prohibidos ( ", ' y \ ). ✅
07. Empty `server` directive ✅
08. Empty `location` vacio ✅
09. 0, 1 o más `server_name` por servidor. ✅
10. 0, 1 o más `listen` por servidor. ✅
11. Duplicado `clietn_max_body_size` actualiza. ✅
12. Misma `location` mismo `server`. ¿Error? ✅ 
13. `method` únicos en `server`. ✅ 
14. Duplicado `alias`, duplicado sobreescribe. ✅
15. Duplicado `root`, duplicado sobreescribe. ✅
16. Duplicado `autoindex`, duplicado sobreescribe, todo salvo `on` 
	lo establece a `off`. ✅
17. Duplicado `index` ✅