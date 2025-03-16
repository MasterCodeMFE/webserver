/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequestHandler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:20 by manufern          #+#    #+#             */
/*   Updated: 2025/03/10 17:44:16 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 // 5️⃣ Leer datos del cliente y responder al cliente

#include "DeployServer.hpp"

static void			close_client(int client_fd);
static std::string	receive_request(int client_fd);
static HttpRequest	parse_request(const std::string& request);
static void			debug_print_http_request(const HttpRequest& httpRequest);
static std::string	read_http_body(int client_fd, const std::string& raw_request, int content_length);

// ========================================
//  FUNCIÓN: handle_client_request
// ========================================
// Maneja la solicitud de un cliente.
//
// Se encarga de:
// 1. Recibir la solicitud HTTP del cliente.
// 2. Analizar la solicitud para extraer método, ruta y encabezados.
// 3. Leer el cuerpo de la solicitud si es necesario.
// 4. Delegar la solicitud a la función que maneja los métodos HTTP.
//
// Parámetros:
// - client_fd: Descriptor del socket del cliente.
// - config: Configuración general del servidor.
//
// Retorno:
// - 0 si la solicitud se procesa correctamente.
// - -1 en caso de error.
int DeployServer::_handle_client_request( int client_fd )
{
	// Recibir solicitud inicial
	std::string raw_request;
	HttpRequest httpRequest;

	raw_request = receive_request(client_fd);
	if (raw_request.empty())
		return -1;
	// Parsear la solicitud HTTP
	httpRequest = parse_request(raw_request);
	debug_print_http_request(httpRequest);

	// Verificar si existe el encabezado "Content-Length" para leer el cuerpo de la solicitud
	std::map<std::string, std::string>::const_iterator it = httpRequest.headers.find("Content-Length");
	if (it != httpRequest.headers.end()) {
		int content_length = 0;
		std::istringstream iss(it->second);
		iss >> content_length;
		std::cout << "Content-Length encontrado: " << content_length << "\n";

		httpRequest.body = read_http_body(client_fd, raw_request, content_length);
		if (!httpRequest.body.empty()) {
			std::cout << "Cuerpo:\n" << httpRequest.body << "\n";
		}
	}

	// Pasar la solicitud completa a la siguiente etapa (por ejemplo, a handle_http_method)
	int res = this->_dispatch_http_request( client_fd, httpRequest );
	return res;
}

// ========================================
//  FUNCIÓN: close_client
// ========================================
// Cierra el socket de un cliente para liberar recursos.
//
// Parámetros:
// - client_fd: Descriptor del socket del cliente.
//
// Retorno: 
// - No tiene retorno.
static void close_client(int client_fd)
{
	close(client_fd);
}

// ========================================
//  FUNCIÓN: receive_request
// ========================================
// Recibe la solicitud HTTP desde el cliente.
//
// Se encarga de:
// 1. Leer los datos enviados por el cliente.
// 2. Manejar la desconexión o errores en la recepción.
//
// Parámetros:
// - client_fd: Descriptor del socket del cliente.
//
// Retorno:
// - La solicitud HTTP como una cadena de texto.
// - Cadena vacía en caso de error o desconexión.
static std::string receive_request(int client_fd)
{
	std::vector<char> buffer(4096);
	ssize_t bytes_received = recv(client_fd, &buffer[0], buffer.size() - 1, 0);

	if (bytes_received == 0)
	{
		std::cout << "Cliente cerró la pestaña o desconectó.\n";
		close_client(client_fd);
		return "";
	} else if (bytes_received < 0)
	{
		std::cerr << "Error en recv().\n";
		return "";
	}

	buffer[bytes_received] = '\0';
	return std::string(&buffer[0], bytes_received);
}

// ========================================
//  FUNCIÓN: parse_request
// ========================================
// Analiza una solicitud HTTP y extrae su método, ruta, protocolo y encabezados.
//
// Parámetros:
// - request: Cadena con la solicitud HTTP cruda.
//
// Retorno:
// - Un objeto HttpRequest con la información extraída.
static HttpRequest parse_request(const std::string& request)
{
	HttpRequest httpRequest;
	std::istringstream stream(request);
	std::string line;
	
	// Extraer la primera línea: método, ruta y protocolo
	if (std::getline(stream, line)) {
		std::istringstream first_line(line);
		first_line >> httpRequest.method >> httpRequest.path >> httpRequest.protocol;
	}

	// Extraer los encabezados
	while (std::getline(stream, line) && line != "\r") {
		std::size_t pos = line.find(": ");
		if (pos != std::string::npos) {
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 2);
			httpRequest.headers[key] = value;
		}
	}

	return httpRequest;
}

// ========================================
//  FUNCIÓN: debug_print_http_request
// ========================================
// Muestra en consola los detalles de la solicitud HTTP.
//
// Parámetros:
// - httpRequest: Objeto HttpRequest con los datos de la solicitud.
//
// Retorno:
// - No tiene retorno.
static void debug_print_http_request(const HttpRequest& httpRequest)
{
	std::cout << "__________CLIENT REQUEST__________\n"
				<< "Método: " << httpRequest.method << "\n"
				<< "Ruta: " << httpRequest.path << "\n"
				<< "Protocolo: " << httpRequest.protocol << "\n"
				<< "Encabezados:\n";
	for (std::map<std::string, std::string>::const_iterator it = httpRequest.headers.begin();
		 it != httpRequest.headers.end(); ++it) {
		std::cout << "\t" << it->first << ": " << it->second << "\n";
	}

	if (!httpRequest.body.empty()) {
		std::cout << "Cuerpo:\n" << httpRequest.body << "\n";
	}
	std::cout << "----------------------------------\n";
}

// ========================================
//  FUNCIÓN: read_http_body
// ========================================
// Lee el cuerpo de la solicitud HTTP si se especifica "Content-Length".
//
// Se encarga de:
// 1. Extraer la parte del cuerpo que ya ha sido recibida.
// 2. Leer el resto del cuerpo desde el socket si es necesario.
//
// Parámetros:
// - client_fd: Descriptor del socket del cliente.
// - raw_request: Solicitud HTTP cruda recibida hasta el momento.
// - content_length: Tamaño del cuerpo esperado.
//
// Retorno:
// - El cuerpo de la solicitud como una cadena de texto.
static std::string read_http_body(int client_fd, const std::string& raw_request, int content_length)
{
	std::size_t pos = raw_request.find("\r\n\r\n");
	std::string body;
	if (pos != std::string::npos) {
		body = raw_request.substr(pos + 4);
	}

	int total_read = body.size();
	while (total_read < content_length) {
		std::vector<char> body_buffer(content_length - total_read);
		int bytes = recv(client_fd, &body_buffer[0], content_length - total_read, 0);
		if (bytes <= 0)
			break;
		body.append(&body_buffer[0], bytes);
		total_read += bytes;
	}

	return body;
}

