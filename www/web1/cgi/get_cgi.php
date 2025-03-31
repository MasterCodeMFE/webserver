#!/usr/bin/php-cgi
<?php
// Configurar la salida HTTP
header("Content-Type: text/html; charset=UTF-8");

echo "<!DOCTYPE html>";
echo "<html lang='es'>";
echo "<head>";
echo "<meta charset='UTF-8'>";
echo "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
echo "<title>Datos Recibidos</title>";
echo "</head>";
echo "<body>";

echo "<h1>Datos Recibidos</h1>";

// Manejar solicitudes GET
if ($_SERVER["REQUEST_METHOD"] === "GET") {
    echo "<h2>Parámetros GET recibidos:</h2>";
    
    if (!empty($_GET)) {
        echo "<ul>";
        foreach ($_GET as $key => $value) {
            $safe_key = htmlspecialchars($key);
            $safe_value = htmlspecialchars($value);
            echo "<li><strong>$safe_key:</strong> $safe_value</li>";
        }
        echo "</ul>";
    } else {
        echo "<p>No se recibieron parámetros GET.</p>";
    }
} 
// Manejar solicitudes POST
elseif ($_SERVER["REQUEST_METHOD"] === "POST") {
    echo "<h2>Datos POST recibidos:</h2>";
    
    $name = isset($_POST["name"]) ? htmlspecialchars($_POST["name"]) : "No proporcionado";
    $email = isset($_POST["email"]) ? htmlspecialchars($_POST["email"]) : "No proporcionado";
    $phone = isset($_POST["phone"]) ? htmlspecialchars($_POST["phone"]) : "No proporcionado";

    echo "<p><strong>Nombre:</strong> $name</p>";
    echo "<p><strong>Correo:</strong> $email</p>";
    echo "<p><strong>Teléfono:</strong> $phone</p>";
} 
// Manejar otros métodos
else {
    echo "<p>Método HTTP no soportado: " . htmlspecialchars($_SERVER["REQUEST_METHOD"]) . "</p>";
}

echo "</body>";
echo "</html>";
?>