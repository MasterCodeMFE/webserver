## Contribución y licencia

### ¿Quieres contribuir?

Las contribuciones son bienvenidas. Por favor, abre un issue o pull request siguiendo las buenas prácticas de desarrollo y manteniendo la calidad del código.

### Autores

- Pablo Abad Aparicio ([pabad-ap](mailto:pabad-ap@student.42.fr))
- Manuel Fernández ([manufern](mailto:manufern@student.42.fr))

### Licencia

Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo LICENSE para más detalles.

---

<p align="center"><b>42 Madrid - 2025</b></p>
## Funcionalidades y endpoints soportados

### Métodos HTTP

- **GET**: Servir archivos estáticos, directorios (con o sin autoindex) y ejecutar scripts CGI.
- **POST**: Procesar formularios, subir archivos y ejecutar scripts CGI con datos.
- **DELETE**: Eliminar archivos o recursos según configuración.

### CGI (Common Gateway Interface)

Permite la ejecución de scripts externos (PHP, Python, etc.) en rutas configuradas. El servidor gestiona el entorno, la entrada/salida y la respuesta HTTP resultante.

### Gestión de errores

- Páginas de error personalizadas por código y por ubicación.
- Respuestas HTTP estándar y mensajes claros para el usuario.

### Otras funcionalidades

- **Autoindex**: Listado automático de archivos en directorios si está habilitado.
- **Redirecciones**: Soporte para redirecciones HTTP configurables.
- **Alias y root**: Flexibilidad en la definición de rutas y mapeo de recursos.
- **Límites configurables**: Tamaño máximo de cuerpo de petición por servidor/location.

---
## Ejecución y ejemplos de uso

### Lanzar el servidor

Puedes iniciar el servidor con un archivo de configuración personalizado:

```bash
./webserv webserv.conf
```
Si no se especifica archivo, se usará la configuración por defecto.

### Ejemplo de archivo de configuración (`webserv.conf`)

```conf
server {
	server_name MiServidor ;
	listen 127.0.0.1:8080 ;
	client_max_body_size 10M ;
	error_page 404 ./www/web1/errors/404.html ;
	root /web1 ;
	location / {
		method GET ;
		method POST ;
	}
	location /cgi {
		method GET ;
		method POST ;
		index php.php ;
	}
}
```

### Pruebas rápidas

- Accede a `http://localhost:8080/` para ver el contenido de `www/web1/index.html`.
- Prueba rutas CGI: `http://localhost:8080/cgi/php.php`
- Sube archivos vía POST o prueba métodos DELETE según la configuración.

---
## Estructura del proyecto

La organización del repositorio está pensada para facilitar la mantenibilidad y escalabilidad del servidor:

```
webserv/
├── Makefile                # Script de compilación principal
├── webserv                 # Ejecutable generado tras compilar
├── *.conf                  # Archivos de configuración de ejemplo
├── include/                # Cabeceras y definiciones de clases principales
├── src/                    # Implementación de la lógica del servidor
│   ├── DeployServer/       # Módulos de despliegue y gestión de conexiones
│   ├── Request/            # Lógica de manejo de peticiones HTTP y CGI
│   ├── parser/             # Parser de archivos de configuración
│   └── ...                 # Otros módulos principales
├── build/                  # Objetos compilados (intermedios)
├── www/                    # DocumentRoot: archivos estáticos, errores y scripts CGI
│   ├── web1/, web2/        # Ejemplos de sitios virtuales
│   └── ...
```

**Principales carpetas:**

- `include/`: Definiciones de clases como `Server`, `Location`, `DeployServer`, `Request`, utilidades y parser.
- `src/`: Implementación de la lógica del servidor, gestión de sockets, eventos, parser y manejo de peticiones.
- `www/`: Contenido web servido por el servidor (HTML, errores, scripts CGI, etc.).
- `build/`: Archivos objeto generados durante la compilación.
- `*.conf`: Archivos de configuración de ejemplo para probar distintas funcionalidades.

---

# Webserv

Webserv es un servidor web HTTP de alto rendimiento, desarrollado íntegramente en C++98 como parte del currículo de 42 Madrid. Este proyecto busca replicar y profundizar en los fundamentos de los servidores web modernos, implementando desde cero la gestión de conexiones, el análisis de peticiones HTTP, la ejecución de scripts CGI y la configuración avanzada mediante archivos `.conf`.

## Características principales

- Cumplimiento del estándar HTTP/1.1
- Soporte multi-servidor y multi-puerto (virtual hosting)
- Configuración granular y flexible mediante archivos `.conf`
- Métodos HTTP soportados: GET, POST, DELETE
- Ejecución de scripts CGI (PHP, Python, etc.)
- Gestión avanzada de errores y páginas personalizadas
- Autoindex configurable para directorios
- Límite de tamaño de cuerpo de petición por servidor/location
- Redirecciones, alias y rutas personalizadas
- Servir archivos estáticos y dinámicos
- Arquitectura basada en sockets, polling y gestión eficiente de eventos

---

## Instalación y compilación

> **Requisitos:**
> - Sistema operativo Linux (recomendado: Ubuntu 22.04+ o entorno 42)
> - Compilador C++ compatible con C++98 (g++ recomendado)
> - Make

Clona el repositorio y compila el proyecto:

```bash
git clone https://github.com/MasterCodeMFE/webserver.git
cd webserver
make
```

El ejecutable `webserv` se generará en el directorio raíz. No se requieren dependencias externas.

---
