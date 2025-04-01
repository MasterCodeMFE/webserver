#!/usr/bin/php-cgi
<?php
// Configurar la salida HTTP
header("Content-Type: text/html; charset=UTF-8");

echo "<!DOCTYPE html>";
echo "<html lang='es'>";
echo "<head><title>Mensaje Recibido</title></head>";
echo "<body>";

// Leer la entrada del POST
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $mensaje = isset($_POST["mensaje"]) ? htmlspecialchars($_POST["mensaje"]) : "No se recibió ningún mensaje.";
    echo "<h1>Mensaje recibido:</h1>";
    echo "<p>$mensaje</p>";
} else {
    echo "<p>Error: No se recibió una solicitud POST.</p>";
}

echo "</body></html>";
?>
