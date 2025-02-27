01. Fichero inexistente ✅ - `./webserv noexist`
02. Fichero vacio ✅ - `./webserv /dev/null`
03. Fichero sin accesos de lectura, escritura, ejecución o ningun permiso ✅
04. Fichero con directiva fuera de zona ✅
05. No poner cierre de sentencia, `;` para tipo E_DIRECTIVE o `}` para tipo E_BLOCK. ✅
06. Caracteres prohibidos ( ", ' y \ ). ✅
07. Empty server directive ✅
08. Listen vacio ❌ - Cierre llaves listen pero no cierre server no salta warning.

