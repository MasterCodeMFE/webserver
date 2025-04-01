#!/usr/bin/php
<?php
header("Content-Type: text/html; charset=UTF-8");

while (true) {
    echo "Bucle infinito...\n";
    flush(); // Envía la salida al navegador
    sleep(1); // Espera 1 segundo para no saturar el servidor
}
?>