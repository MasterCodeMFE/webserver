#include "test.hpp"

// ========================================
//  FUNCIÓN: dispatch_http_request
// ========================================
// Despacha la solicitud HTTP según el método solicitado.
//
// Se encarga de:
// 1. Determinar si la solicitud es para un script CGI y procesarlo si es necesario.
// 2. Manejar solicitudes GET, POST y DELETE llamando a las funciones correspondientes.
// 3. Responder con un error 405 si el método no está permitido.
// 4. Enviar la respuesta generada al cliente.
//
// Parámetros:
// - client_fd: Descriptor del socket del cliente.
// - httpRequest: Objeto que contiene la información de la solicitud HTTP.
// - config: Configuración general del servidor.
//
// Retorno:
// - 0 si la solicitud se procesa correctamente.
int dispatch_http_request(int client_fd, const HttpRequest& httpRequest, const listenSet &config ) {
    std::string response;

    // Verificar si la solicitud es para un script CGI
    if (httpRequest.path.find("/cgi-bin/") == 0) {
        response = handle_cgi("." + httpRequest.path, httpRequest.query_string);
    }
    // Manejar solicitud GET
    else if (httpRequest.method == "GET") {
        response = handle_get(httpRequest, config);
    }
    // Manejar solicitud POST
    else if (httpRequest.method == "POST") {
        response = handle_post(httpRequest);
    }
    // Manejar solicitud DELETE
    else if (httpRequest.method == "DELETE") {
        response = handle_delete(httpRequest);
    }
    // Responder con error 405 si el método no es reconocido
    else {
        std::string error = "Método no permitido\n";
        response = build_http_response(error, "text/plain", 405);
    }

    // Enviar la respuesta al cliente
    std::cout << "\U0001F4E4 Enviando respuesta...\n";
    send_all(client_fd, response.c_str(), response.size());

    return 0;
}
