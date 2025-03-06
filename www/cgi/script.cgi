#!/usr/bin/python3
import os
import sys

print("Content-Type: text/html")
print()  # Línea en blanco requerida por CGI

print("<html><body>")
print("<h1>¡Hola desde CGI!</h1>")
print("<p>Query String: " + os.getenv("QUERY_STRING", "") + "</p>")
print("</body></html>")