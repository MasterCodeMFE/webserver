#!/usr/bin/php
<?php
header("Content-Type: text/html; charset=UTF-8");

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_FILES['file'])) {
    $upload_dir = "/tmp/"; // Cambia esto a tu directorio de destino
    $upload_file = $upload_dir . basename($_FILES['file']['name']);

    if (move_uploaded_file($_FILES['file']['tmp_name'], $upload_file)) {
        echo "Archivo subido con éxito: " . htmlspecialchars($upload_file);
    } else {
        echo "Error al subir el archivo.";
    }
} else {
    // Formulario de subida
    echo '<form action="upload.php" method="POST" enctype="multipart/form-data">
            <input type="file" name="file">
            <input type="submit" value="Subir archivo">
          </form>';
}
?>