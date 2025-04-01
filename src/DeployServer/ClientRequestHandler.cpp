/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequestHandler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:20 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 20:09:25 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "DeployServer.hpp"
#include "Request.hpp"

static void			close_client(int client_fd);
static std::string	receive_request(int client_fd);
static HttpRequest	parse_request(const std::string& request);
static std::string	read_http_body(int client_fd, const std::string& raw_request, int content_length);

int DeployServer::_handle_client_request( int client_fd )
{
	std::string raw_request;
	HttpRequest httpRequest;

	raw_request = receive_request(client_fd);
	if (raw_request.empty())
		return -1;
	httpRequest = parse_request(raw_request);

	std::map<std::string, std::string>::const_iterator it = httpRequest.headers.find("Content-Length");
	if (it != httpRequest.headers.end()) {
		int content_length = 0;
		std::istringstream iss(it->second);
		iss >> content_length;

		httpRequest.body = read_http_body(client_fd, raw_request, content_length);
	}
	int res = this->_dispatch_http_request( client_fd, httpRequest );
	return res;
}

void Request::fake_usleep(int milliseconds)
{
    clock_t start_time = clock();
    while (static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC < milliseconds / 1000.0)
    {

    }
}
static void close_client(int client_fd)
{
	close(client_fd);
}

static std::string receive_request(int client_fd)
{
    std::string msg;
    char buffer[4096];
    ssize_t bytes_received;
    int attempts = 15;
    const int delay_ms = 100;

    while (true)
    {

        bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received < 0)
        {
            attempts--;

            if (attempts > 0)
            {
                Request::fake_usleep(delay_ms);
                continue;
            }
            else
            {
                close_client(client_fd);
                return "";
            }
        }
        else if (bytes_received == 0)
        {

            std::cerr << "Cliente cerró la conexión." << std::endl;
            break;
        }

        buffer[bytes_received] = '\0';
        msg.append(buffer, bytes_received);
        std::size_t pos = msg.find("\r\n\r\n");
        if (pos != std::string::npos)
        {
            std::map<std::string, std::string> headers;
            std::istringstream stream(msg.substr(0, pos));
            std::string line;

            while (std::getline(stream, line) && line != "\r")
            {
                std::size_t delimiter = line.find(": ");
                if (delimiter != std::string::npos)
                {
                    std::string key = line.substr(0, delimiter);
                    std::string value = line.substr(delimiter + 2);
                    headers[key] = value;
                }
            }
            if (headers.find("Content-Length") != headers.end())
            {
                int content_length = atoi(headers["Content-Length"].c_str());
                std::size_t body_start = pos + 4;
                std::size_t body_received = msg.size() - body_start;

                attempts = 15;
                while (body_received < static_cast<std::size_t>(content_length))
                {
                    bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
                    if (bytes_received < 0)
                    {
                        attempts--;
                        if (attempts > 0)
                        {
                            Request::fake_usleep(delay_ms);
                            continue;
                        }
                        else
                        {
                            close_client(client_fd);
                            return "";
                        }
                    }
                    else if (bytes_received == 0)
                    {
                        std::cerr << "Cliente cerró la conexión durante la recepción del cuerpo." << std::endl;
                        close_client(client_fd);
                        return "";
                    }

                    buffer[bytes_received] = '\0';
                    msg.append(buffer, bytes_received);
                    body_received += bytes_received;
                }
            }

            break;
        }
    }

    return msg;
}

static std::string url_decode(const std::string& encoded) {
    std::string decoded;
    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 < encoded.length()) {
                std::stringstream ss;
                ss << std::hex << encoded.substr(i + 1, 2);
                unsigned int code;
                ss >> code;
                decoded += static_cast<char>(code);
                i += 2;
            } else {
                return encoded;
            }
        } else {
            decoded += encoded[i];
        }
    }
    return decoded;
}

static HttpRequest parse_request(const std::string& request) {
    HttpRequest httpRequest;
    std::istringstream stream(request);
    std::string line;
    if (std::getline(stream, line)) {
        std::istringstream first_line(line);
        first_line >> httpRequest.method >> httpRequest.path >> httpRequest.protocol;
        httpRequest.path = url_decode(httpRequest.path);
    }

    while (std::getline(stream, line) && line != "\r" && line != "") {
        std::size_t pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2);
            httpRequest.headers[key] = value;
        }
    }

    while (std::getline(stream, line)) {
        httpRequest.body += line + "\n";
    }

    if (!httpRequest.body.empty() && httpRequest.body[httpRequest.body.size() - 1] == '\n') {
        httpRequest.body.erase(httpRequest.body.size() - 1);
    }
    return httpRequest;
}

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

std::string DeployServer::_handle_redirection(int status_code, const std::string& location, const Location& location_config)
{
    std::ostringstream response;
    if (status_code == 301 || status_code == 302 || status_code == 307 || status_code == 308)
    {
        std::string status_message;
        switch (status_code)
        {
            case 301: status_message = "Moved Permanently"; break;
            case 302: status_message = "Found"; break;
            case 307: status_message = "Temporary Redirect"; break;
            case 308: status_message = "Permanent Redirect"; break;
        }

        response << "HTTP/1.1 " << status_code << " " << status_message << "\r\n"
                 << "Location: " << location << "\r\n"
                 << "Content-Length: 0\r\n"
                 << "Connection: close\r\n\r\n";
    }
    else
    {
        return location_config.getErrorPage(500);
    }

    return response.str();
}