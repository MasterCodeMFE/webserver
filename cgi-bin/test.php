#!/usr/bin/php
<?php
header("Content-Type: text/html");
echo "<html><body><h1>CGI en PHP funcionando!</h1>";
echo "<p>QUERY_STRING: " . getenv("QUERY_STRING") . "</p>";
echo "</body></html>";
?>
