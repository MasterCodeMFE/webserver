#!/bin/bash

echo "Content-Type: text/html"
echo
echo "<html><body><h1>CGI en Bash funcionando!</h1>"
echo "<p>QUERY_STRING: $QUERY_STRING</p>"
echo "</body></html>"
