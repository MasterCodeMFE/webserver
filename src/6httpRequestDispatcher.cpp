#include "test.hpp"

// -----------------------------------------------------------------------------
// Función principal para gestionar la solicitud (paso_seis)
// -----------------------------------------------------------------------------

int dispatch_http_request(int client_fd, const HttpRequest& httpRequest, const Config &config) {
    std::string response;
    if (httpRequest.path.find("/cgi-bin/") == 0) {
        response = handle_cgi("." + httpRequest.path, httpRequest.query_string);
    }
    else if (httpRequest.method == "GET") {
        response = handle_get(httpRequest, config);
    }
    else if (httpRequest.method == "POST") {
        response = handle_post(httpRequest);
    }
    else if (httpRequest.method == "DELETE") {
        response = handle_delete(httpRequest);
    }
    else {
        std::string error = "Metodo no permitido\n";
        response = build_http_response(error, "text/plain", 405);
    }

    std::cout << "\U0001F4E4 Enviando respuesta...\n";
    send_all(client_fd, response.c_str(), response.size());
    return 0;
}
