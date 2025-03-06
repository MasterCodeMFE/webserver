/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:13 by manufern          #+#    #+#             */
/*   Updated: 2025/03/06 19:12:14 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// ========================================
//  FUNCIÓN: read_file
// ========================================
// Lee un archivo desde el disco y devuelve su contenido en una cadena.
//
// Parámetros:
// - filepath: Ruta del archivo a leer.
//
// Retorno:
// - El contenido del archivo como una cadena de caracteres.
// - Si no se puede abrir el archivo, retorna una cadena vacía.
std::string Request::_read_file(const std::string& filepath)
{
    std::ifstream file(filepath.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// ========================================
//  FUNCIÓN: int_to_string
// ========================================
// Convierte un número entero en una cadena de caracteres.
//
// Compatible con C++98, ya que usa stringstream en lugar de std::to_string().
//
// Parámetros:
// - number: Número entero a convertir.
//
// Retorno:
// - Representación en cadena del número.
std::string Request::_int_to_string(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

// ========================================
//  FUNCIÓN: get_content_type
// ========================================
// Determina el tipo de contenido (MIME type) basado en la extensión del archivo.
//
// Parámetros:
// - filepath: Ruta del archivo cuyo tipo de contenido se quiere obtener.
//
// Retorno:
// - El tipo MIME correspondiente a la extensión del archivo.
// - Si la extensión no está en la lista, devuelve "application/octet-stream".

std::string Request::_get_content_type(const std::string& filepath)
{
    std::map<std::string, std::string> mime_types;
    mime_types[".html"] = "text/html";
    mime_types[".css"] = "text/css";
    mime_types[".js"] = "application/javascript";
    mime_types[".png"] = "image/png";
    mime_types[".jpg"] = "image/jpeg";
    mime_types[".jpeg"] = "image/jpeg";
    mime_types[".gif"] = "image/gif";
    mime_types[".txt"] = "text/plain";

    size_t dot_pos = filepath.find_last_of(".");
    if (dot_pos != std::string::npos)
    {
        std::string ext = filepath.substr(dot_pos);
        if (mime_types.find(ext) != mime_types.end())
        {
            return mime_types[ext];
        }
    }
    return "application/octet-stream"; // Tipo por defecto
}

// ========================================
//  FUNCIÓN: build_http_response
// ========================================
// Construye una respuesta HTTP a partir del contenido, el tipo MIME y el código de estado.
//
// Parámetros:
// - content: Cuerpo del mensaje HTTP (contenido del archivo).
// - content_type: Tipo de contenido del archivo (MIME).
// - status_code: Código de estado HTTP.
//
// Retorno:
// - Cadena con la respuesta HTTP completa, incluyendo encabezados y cuerpo.

std::string Request::build_http_response(const std::string& content, const std::string& content_type, int status_code)
{
    std::string status_text;
    switch (status_code)
    {
        case 200: status_text = "OK"; break;
        case 400: status_text = "Bad Request"; break;
        case 403: status_text = "Forbidden"; break;
        case 404: status_text = "Not Found"; break;
        case 405: status_text = "Method Not Allowed"; break;
        case 500: status_text = "Internal Server Error"; break;
        default: status_text = "OK"; break;
    }
    return "HTTP/1.1 " + Request::_int_to_string(status_code) + " " + status_text + "\r\n"
           "Content-Type: " + content_type + "\r\n"
           "Content-Length: " + Request::_int_to_string(content.size()) + "\r\n\r\n" + content;
}

// ========================================
//  FUNCIÓN: send_all
// ========================================
// Envía todos los datos a través de un socket, asegurando que se envíe
// la cantidad total de bytes esperada.
//
// Parámetros:
// - sockfd: Descriptor del socket de conexión.
// - buffer: Puntero al buffer que contiene los datos a enviar.
// - length: Longitud total de los datos a enviar.
//
// Retorno:
// - Número total de bytes enviados si la operación es exitosa.
// - Retorna -1 si ocurre un error.

ssize_t Request::send_all(int sockfd, const char* buffer, size_t length)
{
    size_t total_sent = 0;
    while (total_sent < length)
    {
        ssize_t bytes_sent = send(sockfd, buffer + total_sent, length - total_sent, 0);
        if (bytes_sent <= 0)
        {
            std::cerr << "❌ Error enviando datos al cliente." << std::endl;
            return -1;
        }
        total_sent += bytes_sent;
    }
    return total_sent;
}