#!/usr/bin/php
<?php
header("Content-Type: text/html; charset=UTF-8");

$nombre = "Juan";
$edad = 25;
$url = "/cgi-bin/get_cgi.php?nombre=" . urlencode($nombre) . "&edad=" . urlencode($edad);

echo "<html><body>";
echo "<h1>Generación de URL con GET</h1>";
echo "<p>Haz clic en el enlace para enviar datos con GET:</p>";
echo "<a href='$url'>$url</a>";
echo "</body></html>";
?>