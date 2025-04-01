#!/usr/bin/php
<?php
// Asegúrate de que el script tenga permisos de ejecución (chmod +x mi_script.cgi)

// Establece el encabezado de contenido para UTF-8
header("Content-Type: text/html; charset=UTF-8");

// Obtiene los datos del formulario
$name = isset($_POST['name']) ? $_POST['name'] : '';
$email = isset($_POST['email']) ? $_POST['email'] : '';
$phone = isset($_POST['phone']) ? $_POST['phone'] : '';

// Procesar los datos (aquí simplemente se muestran)
echo "<html><body>";
echo "<h1>Datos Recibidos</h1>";
echo "<p>Nombre: " . htmlspecialchars($name, ENT_QUOTES, 'UTF-8') . "</p>";
echo "<p>Correo Electrónico: " . htmlspecialchars($email, ENT_QUOTES, 'UTF-8') . "</p>";
echo "<p>Número de Teléfono: " . htmlspecialchars($phone, ENT_QUOTES, 'UTF-8') . "</p>";
echo "</body></html>";
?>
