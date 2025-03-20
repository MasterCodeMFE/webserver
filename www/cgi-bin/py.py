#!/usr/bin/python3
import os
import sys
import json
import cgi
import cgitb
import html
import urllib.parse
from datetime import datetime

cgitb.enable()  # Habilita depuración en CGI

LOG_FILE = "/tmp/cgi_log.txt"  # Evitar escribir en el directorio del script por permisos

# 🔹 Función para escribir logs
def write_log(message):
    with open(LOG_FILE, "a") as log:
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        log.write(f"[{now}] {message}\n")

# 🔹 Obtener método de la petición
method = os.environ.get("REQUEST_METHOD", "GET")

# 🔹 Leer datos de QUERY_STRING (GET)
query_string = os.environ.get("QUERY_STRING", "")

# 🔹 Analizar parámetros GET
get_params = urllib.parse.parse_qs(query_string)

# 🔹 Leer datos de POST de forma segura
post_data = ""
content_length = os.environ.get("CONTENT_LENGTH", "0")
try:
    content_length = int(content_length) if content_length else 0
    if content_length > 0:
        post_data = sys.stdin.read(content_length)
except ValueError:
    content_length = 0

# 🔹 Intentar parsear JSON en POST si existe
post_json = {}
if post_data:
    try:
        post_json = json.loads(post_data)
    except json.JSONDecodeError:
        pass  # No es JSON válido, lo dejamos como string

# 🔹 Obtener la fecha y hora del servidor
current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

# 🔹 Determinar si el cliente quiere JSON
accept_header = os.environ.get("HTTP_ACCEPT", "")
wants_json = "application/json" in accept_header

# 🔹 Escribir log de la petición
write_log(f"Método: {method} | Query: {query_string} | POST: {post_data}")

# 🔹 Generar respuesta en JSON o HTML según lo que quiera el cliente
if wants_json:
    response = {
        "status": "ok",
        "time": current_time,
        "method": method,
        "query": get_params,
        "post": post_json if post_json else post_data
    }
    print("Content-Type: application/json\n")
    print(json.dumps(response, indent=4))
else:
    print("Content-Type: text/html\n")
    print(f"""
    <html>
    <head>
        <title>CGI Avanzado</title>
        <meta charset="UTF-8">
        <style>
            body {{ font-family: Arial, sans-serif; margin: 20px; }}
            pre {{ background: #f4f4f4; padding: 10px; border-radius: 5px; overflow: auto; }}
        </style>
    </head>
    <body>
        <h1>¡Hola desde CGI Mejorado!</h1>
        <p><b>Hora del Servidor:</b> {current_time}</p>
        <p><b>Método HTTP:</b> {html.escape(method)}</p>
        <p><b>Query String:</b> {html.escape(query_string)}</p>
        <p><b>Datos POST:</b></p>
        <pre>{html.escape(post_data) if not post_json else json.dumps(post_json, indent=4)}</pre>
        <h2>Variables de Entorno</h2>
        <pre>{json.dumps(dict(os.environ), indent=4)}</pre>
    </body>
    </html>
    """)
