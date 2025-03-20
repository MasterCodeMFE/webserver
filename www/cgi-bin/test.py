#!/usr/bin/env python3

import os

print("Content-Type: text/html\r\n\r\n")
print("<html><body><h1>CGI en Python funcionando!</h1>")
print(f"<p>QUERY_STRING: {os.getenv('QUERY_STRING', '')}</p>")
print("</body></html>")
