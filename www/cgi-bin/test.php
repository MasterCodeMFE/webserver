#!/usr/bin/php
<?php
// Habilitar reportes de error para facilitar la depuración
error_reporting(E_ALL);
ini_set('display_errors', 1);

// Iniciar la sesión (si el entorno lo permite)
session_start();

// Si se solicita limpiar la sesión, se borra y se redirige
if (isset($_GET['clear_session'])) {
    session_unset();
    session_destroy();
    header("Location: " . $_SERVER['SCRIPT_NAME']);
    exit;
}

// Procesar el envío de un archivo (en formulario POST con enctype multipart/form-data)
$uploadResult = "";
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_FILES['uploaded_file'])) {
    if ($_FILES['uploaded_file']['error'] === UPLOAD_ERR_OK) {
        $uploadDir = sys_get_temp_dir();
        $uploadFile = $uploadDir . '/' . basename($_FILES['uploaded_file']['name']);
        if (move_uploaded_file($_FILES['uploaded_file']['tmp_name'], $uploadFile)) {
            $uploadResult = "Archivo subido correctamente a: " . htmlspecialchars($uploadFile, ENT_QUOTES, 'UTF-8');
        } else {
            $uploadResult = "Error al mover el archivo subido.";
        }
    } else {
        $uploadResult = "Error en la subida del archivo: " . $_FILES['uploaded_file']['error'];
    }
}

// Procesar el envío de datos vía formulario para establecer cookies (por GET)
if (isset($_GET['cookie_name']) && isset($_GET['cookie_value'])) {
    $cookie_name = $_GET['cookie_name'];
    $cookie_value = $_GET['cookie_value'];
    setcookie($cookie_name, $cookie_value, time() + 3600); // Válida por 1 hora
    // Recargar la página para mostrar la cookie establecida
    header("Location: " . $_SERVER['SCRIPT_NAME']);
    exit;
}

// Enviar la cabecera HTTP con el tipo de contenido y la codificación
header("Content-Type: text/html; charset=UTF-8");

// Comenzar la salida HTML
echo "<!DOCTYPE html>";
echo "<html lang='es'>";
echo "<head>";
echo "  <meta charset='UTF-8'>";
echo "  <title>CGI en PHP - Implementación Compleja</title>";
echo "  <style>
            body { font-family: Arial, sans-serif; margin: 20px; background-color: #f2f2f2; }
            .section { background: #fff; padding: 15px; margin-bottom: 20px; border: 1px solid #ccc; }
            table { border-collapse: collapse; width: 100%; margin-bottom: 20px; }
            th, td { border: 1px solid #666; padding: 8px; text-align: left; }
            th { background-color: #ddd; }
            h1, h2 { color: #333; }
            .form-section { margin-bottom: 30px; }
        </style>";
echo "</head>";
echo "<body>";

echo "<h1>CGI en PHP - Implementación Compleja</h1>";

// Sección: Información de la Solicitud
echo "<div class='section'>";
echo "  <h2>Información de la Solicitud</h2>";
echo "  <p><strong>Método:</strong> " . htmlspecialchars($_SERVER['REQUEST_METHOD'], ENT_QUOTES, 'UTF-8') . "</p>";
echo "  <p><strong>Query String:</strong> " . htmlspecialchars($_SERVER['QUERY_STRING'], ENT_QUOTES, 'UTF-8') . "</p>";
echo "  <p><strong>Script Name:</strong> " . htmlspecialchars($_SERVER['SCRIPT_NAME'], ENT_QUOTES, 'UTF-8') . "</p>";
echo "  <p><strong>Fecha y hora:</strong> " . date('Y-m-d H:i:s') . "</p>";
echo "  <p><strong>Directorio actual:</strong> " . getcwd() . "</p>";
echo "</div>";

// Sección: Parámetros GET
echo "<div class='section'>";
echo "  <h2>Parámetros GET</h2>";
if (!empty($_GET)) {
    echo "  <table>";
    echo "    <tr><th>Clave</th><th>Valor</th></tr>";
    foreach ($_GET as $key => $value) {
        if ($key == 'cookie_name' || $key == 'cookie_value' || $key == 'clear_session') continue;
        echo "    <tr><td>" . htmlspecialchars($key, ENT_QUOTES, 'UTF-8') . "</td><td>" . htmlspecialchars($value, ENT_QUOTES, 'UTF-8') . "</td></tr>";
    }
    echo "  </table>";
} else {
    echo "  <p>No se han recibido parámetros GET.</p>";
}
echo "</div>";

// Sección: Parámetros POST
echo "<div class='section'>";
echo "  <h2>Parámetros POST</h2>";
if ($_SERVER['REQUEST_METHOD'] === 'POST' && !empty($_POST)) {
    echo "  <table>";
    echo "    <tr><th>Clave</th><th>Valor</th></tr>";
    foreach ($_POST as $key => $value) {
        if ($key == 'uploaded_file') continue;
        echo "    <tr><td>" . htmlspecialchars($key, ENT_QUOTES, 'UTF-8') . "</td><td>" . htmlspecialchars($value, ENT_QUOTES, 'UTF-8') . "</td></tr>";
    }
    echo "  </table>";
} else {
    echo "  <p>No se han recibido parámetros POST.</p>";
}
echo "</div>";

// Sección: Subida de Archivo
if (!empty($uploadResult)) {
    echo "<div class='section'>";
    echo "  <h2>Resultado de la Subida de Archivo</h2>";
    echo "  <p>" . $uploadResult . "</p>";
    echo "</div>";
}

// Sección: Variables de Entorno
echo "<div class='section'>";
echo "  <h2>Variables de Entorno</h2>";
echo "  <table>";
echo "    <tr><th>Variable</th><th>Valor</th></tr>";
$envVars = array(
    'REQUEST_METHOD', 'QUERY_STRING', 'REMOTE_ADDR', 'SERVER_NAME', 
    'SERVER_PORT', 'SCRIPT_NAME', 'HTTP_USER_AGENT', 'HTTP_HOST', 'CONTENT_TYPE'
);
foreach ($envVars as $var) {
    echo "    <tr><td>" . htmlspecialchars($var, ENT_QUOTES, 'UTF-8') . "</td><td>" . htmlspecialchars(getenv($var), ENT_QUOTES, 'UTF-8') . "</td></tr>";
}
echo "  </table>";
echo "</div>";

// Sección: Cookies
echo "<div class='section'>";
echo "  <h2>Cookies</h2>";
if (!empty($_COOKIE)) {
    echo "  <table>";
    echo "    <tr><th>Nombre</th><th>Valor</th></tr>";
    foreach ($_COOKIE as $name => $value) {
        echo "    <tr><td>" . htmlspecialchars($name, ENT_QUOTES, 'UTF-8') . "</td><td>" . htmlspecialchars($value, ENT_QUOTES, 'UTF-8') . "</td></tr>";
    }
    echo "  </table>";
} else {
    echo "  <p>No hay cookies establecidas.</p>";
}
echo "</div>";

// Sección: Datos de Sesión
echo "<div class='section'>";
echo "  <h2>Datos de Sesión</h2>";
if (!empty($_SESSION)) {
    echo "  <table>";
    echo "    <tr><th>Clave</th><th>Valor</th></tr>";
    foreach ($_SESSION as $key => $value) {
        echo "    <tr><td>" . htmlspecialchars($key, ENT_QUOTES, 'UTF-8') . "</td><td>" . htmlspecialchars($value, ENT_QUOTES, 'UTF-8') . "</td></tr>";
    }
    echo "  </table>";
} else {
    echo "  <p>No hay datos de sesión almacenados.</p>";
}
echo "  <p><a href='?clear_session=1'>Limpiar sesión</a></p>";
echo "</div>";

// Sección: Formularios Interactivos

// Formulario para envío de datos (POST) y subida de archivos
echo "<div class='section form-section'>";
echo "  <h2>Formulario de Envío (POST y Archivo)</h2>";
echo "  <form action='" . htmlspecialchars($_SERVER['SCRIPT_NAME'], ENT_QUOTES, 'UTF-8') . "' method='post' enctype='multipart/form-data'>";
echo "    <label for='nombre'>Nombre:</label><br>";
echo "    <input type='text' id='nombre' name='nombre' required><br><br>";
echo "    <label for='comentario'>Comentario:</label><br>";
echo "    <textarea id='comentario' name='comentario' rows='4' cols='50'></textarea><br><br>";
echo "    <label for='uploaded_file'>Subir archivo:</label><br>";
echo "    <input type='file' id='uploaded_file' name='uploaded_file'><br><br>";
echo "    <input type='submit' value='Enviar'>";
echo "  </form>";
echo "</div>";

// Formulario para establecer cookies
echo "<div class='section form-section'>";
echo "  <h2>Establecer Cookies</h2>";
echo "  <form action='" . htmlspecialchars($_SERVER['SCRIPT_NAME'], ENT_QUOTES, 'UTF-8') . "' method='get'>";
echo "    <label for='cookie_name'>Nombre de Cookie:</label><br>";
echo "    <input type='text' id='cookie_name' name='cookie_name' required><br><br>";
echo "    <label for='cookie_value'>Valor de Cookie:</label><br>";
echo "    <input type='text' id='cookie_value' name='cookie_value' required><br><br>";
echo "    <input type='submit' value='Establecer Cookie'>";
echo "  </form>";
echo "</div>";

echo "</body>";
echo "</html>";
?>
