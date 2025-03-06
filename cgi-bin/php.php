<?php
header("Content-Type: text/html; charset=UTF-8");

// 🔹 Log de depuración
$logFile = "/tmp/cgi_php_log.txt";
function write_log($message) {
    global $logFile;
    file_put_contents($logFile, "[" . date("Y-m-d H:i:s") . "] " . $message . "\n", FILE_APPEND);
}

// 🔹 Obtener datos de la petición
$method = $_SERVER["REQUEST_METHOD"] ?? "GET";
$query_string = $_SERVER["QUERY_STRING"] ?? "";
$get_params = $_GET;
$post_data = file_get_contents("php://input");
$post_json = json_decode($post_data, true);
$post_type = "text/plain";

// 🔹 Determinar tipo de contenido
if ($post_data) {
    if ($post_json) {
        $post_type = "application/json";
    } elseif (stripos($post_data, "<") === 0) {
        $post_type = "application/xml";
    }
}

// 🔹 Obtener la fecha y hora del servidor
$current_time = date("Y-m-d H:i:s");

// 🔹 Obtener cabeceras HTTP
$headers = getallheaders();

// 🔹 Determinar si el cliente quiere JSON
$wants_json = isset($headers["Accept"]) && strpos($headers["Accept"], "application/json") !== false;

// 🔹 Escribir log detallado de la petición
write_log("Método: $method | Query: $query_string | POST: $post_data ($post_type) | Headers: " . json_encode($headers));

// 🔹 Responder con JSON si es necesario
if ($wants_json) {
    header("Content-Type: application/json");
    echo json_encode([
        "status" => "ok",
        "time" => $current_time,
        "method" => $method,
        "query" => $get_params,
        "post" => $post_json ?: $post_data,
        "post_type" => $post_type,
        "headers" => $headers
    ], JSON_PRETTY_PRINT);
    exit;
}

// 🔹 Respuesta en HTML
?>
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>CGI Avanzado PHP</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        pre { background: #f4f4f4; padding: 10px; border-radius: 5px; overflow: auto; }
    </style>
</head>
<body>
    <h1>¡Hola desde CGI Mejorado en PHP!</h1>
    <p><b>Hora del Servidor:</b> <?= htmlspecialchars($current_time) ?></p>
    <p><b>Método HTTP:</b> <?= htmlspecialchars($method) ?></p>
    <p><b>Query String:</b> <?= htmlspecialchars($query_string) ?></p>
    <p><b>Datos POST (<?= htmlspecialchars($post_type) ?>):</b></p>
    <pre><?= htmlspecialchars($post_json ? json_encode($post_json, JSON_PRETTY_PRINT) : $post_data) ?></pre>
    <h2>Cabeceras HTTP</h2>
    <pre><?= htmlspecialchars(json_encode($headers, JSON_PRETTY_PRINT)) ?></pre>
</body>
</html>
