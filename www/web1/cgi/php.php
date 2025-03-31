#!/usr/bin/php
<?php
header("Content-Type: text/html; charset=UTF-8");

// Asegura que el script se ejecuta correctamente como CGI
if (php_sapi_name() != "cgi-fcgi") {
    exit("Este script debe ejecutarse como CGI.");
}

$nombre = "Juan";
$edad = 25;

// Construcción de la URL (ajusta la ruta base según tu servidor)
$url = "./get_cgi.php?nombre=" . urlencode($nombre) . "&edad=" . urlencode($edad);

echo "<html><body>";
echo "<h1>Generación de URL con GET</h1>";
echo "<p>Haz clic en el enlace para enviar datos con GET:</p>";
echo "<a href='$url'>$url</a>";
echo "</body></html>";
?>










