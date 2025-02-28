/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:44:28 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/21 11:25:04 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Status.hpp"


/** Variable that stores default status codes and their default description */
std::map<int, std::string>	Status::_m_status_responses;

/** Non member function to load default status codes and descriptions (responses) */
void	Status::_setStatusResponses( void )
{
	//1xx informational response
	Status::_m_status_responses.insert(std::make_pair(100, "Continue"));
	Status::_m_status_responses.insert(std::make_pair(101, "Switching Protocols"));
	Status::_m_status_responses.insert(std::make_pair(102, "Processing (WebDAV; RFC 2518)"));
	Status::_m_status_responses.insert(std::make_pair(103, "Early Hints (RFC 8297)"));
	
	//2xx success
	Status::_m_status_responses.insert(std::make_pair(200, "OK"));
	Status::_m_status_responses.insert(std::make_pair(201, "Created"));
	Status::_m_status_responses.insert(std::make_pair(202, "Accepted"));
	Status::_m_status_responses.insert(std::make_pair(203, "Non-Authoritative Information (since HTTP/1.1)"));
	Status::_m_status_responses.insert(std::make_pair(204, "No Content"));
	Status::_m_status_responses.insert(std::make_pair(205, "Reset Content"));
	Status::_m_status_responses.insert(std::make_pair(206, "Partial Content"));
	Status::_m_status_responses.insert(std::make_pair(207, "Multi-Status (WebDAV, RFC 4218)"));
	Status::_m_status_responses.insert(std::make_pair(208, "Already Reported (WebDAV, RFC 5842)"));
	Status::_m_status_responses.insert(std::make_pair(226, "IM Used (RFC 3229)"));
	
	//3xx redirection
	Status::_m_status_responses.insert(std::make_pair(300, "Multiple Choices"));
	Status::_m_status_responses.insert(std::make_pair(301, "Moved Permanently"));
	Status::_m_status_responses.insert(std::make_pair(302, "Found (Previously \"Moved temporarily\")"));
	Status::_m_status_responses.insert(std::make_pair(303, "See Other (since HTTP/1.1)"));
	Status::_m_status_responses.insert(std::make_pair(304, "Not Modified"));
	Status::_m_status_responses.insert(std::make_pair(305, "Use Proxy (since HTTP/1.1)"));
	Status::_m_status_responses.insert(std::make_pair(306, "Switch Proxy"));
	Status::_m_status_responses.insert(std::make_pair(307, "Temporary Redirect (since HTTP/1.1)"));
	Status::_m_status_responses.insert(std::make_pair(308, "Permanent Redirect"));
		
	//4xx client errors
	Status::_m_status_responses.insert(std::make_pair(400, "Bad Request"));
	Status::_m_status_responses.insert(std::make_pair(401, "Unauthorized"));
	Status::_m_status_responses.insert(std::make_pair(402, "Payment Required"));
	Status::_m_status_responses.insert(std::make_pair(403, "Forbidden"));
	Status::_m_status_responses.insert(std::make_pair(404, "Not Found"));
	Status::_m_status_responses.insert(std::make_pair(405, "Method Not Allowed"));
	Status::_m_status_responses.insert(std::make_pair(406, "Not Acceptable"));
	Status::_m_status_responses.insert(std::make_pair(407, "Proxy Authentication Required"));
	Status::_m_status_responses.insert(std::make_pair(408, "Request Timeout"));
	Status::_m_status_responses.insert(std::make_pair(409, "Conflict"));
	Status::_m_status_responses.insert(std::make_pair(410, "Gone"));
	Status::_m_status_responses.insert(std::make_pair(411, "Length  Required"));
	Status::_m_status_responses.insert(std::make_pair(412, "Precondition Failed"));
	Status::_m_status_responses.insert(std::make_pair(413, "Payload Too Large"));
	Status::_m_status_responses.insert(std::make_pair(414, "URI Too Long"));
	Status::_m_status_responses.insert(std::make_pair(415, "Unsupported Media Type"));
	Status::_m_status_responses.insert(std::make_pair(416, "Range Not Satisfiable"));
	Status::_m_status_responses.insert(std::make_pair(417, "Expectation Failed"));
	Status::_m_status_responses.insert(std::make_pair(418, "I'm a teapot (RFC 2324, RFC 7168)"));
	Status::_m_status_responses.insert(std::make_pair(421, "Misdirected Request"));
	Status::_m_status_responses.insert(std::make_pair(422, "Unprocessable Content"));
	Status::_m_status_responses.insert(std::make_pair(423, "Locked (WebDAV,RFC 4918)"));
	Status::_m_status_responses.insert(std::make_pair(424, "Failed Dependency (WebDAV,RFC 4918)"));
	Status::_m_status_responses.insert(std::make_pair(425, "Too Early (RFC 8470)"));
	Status::_m_status_responses.insert(std::make_pair(426, "Upgrade Required"));
	Status::_m_status_responses.insert(std::make_pair(428, "Precondition Required (RFC 6585)"));
	Status::_m_status_responses.insert(std::make_pair(429, "Too Many Requests (RFC 6585)"));
	Status::_m_status_responses.insert(std::make_pair(431, "Request Header Field Too Large (RFC 6585)"));
	Status::_m_status_responses.insert(std::make_pair(451, "Unavailable For Legal Reasons (RFC 7725)"));
	
	//5xx server errors
	Status::_m_status_responses.insert(std::make_pair(500, "Internal Server Error"));
	Status::_m_status_responses.insert(std::make_pair(501, "Not Implemented"));
	Status::_m_status_responses.insert(std::make_pair(502, "Bad Gateway"));
	Status::_m_status_responses.insert(std::make_pair(503, "Service Unavailable"));
	Status::_m_status_responses.insert(std::make_pair(504, "Gateway Timeout"));
	Status::_m_status_responses.insert(std::make_pair(505, "HTTP Version Not Supported"));
	Status::_m_status_responses.insert(std::make_pair(506, "Variant Also Negotiates (RFC 2295)"));
	Status::_m_status_responses.insert(std::make_pair(507, "Insufficient Storage (WebDAV,RFC 4918)"));
	Status::_m_status_responses.insert(std::make_pair(508, "Loop Detected  (WebDAV)"));
	Status::_m_status_responses.insert(std::make_pair(509, "Bandwidth Limit Exceeded"));
	Status::_m_status_responses.insert(std::make_pair(510, "Not Extended (RFC 2774)"));
	Status::_m_status_responses.insert(std::make_pair(511, "Network Authentication Required"));
}

Status::Status( void ){}

Status::Status( Status const &src ){(void)src;}

Status::~Status( void ){}

Status	&Status::operator=( Status const &src ){ (void)src; return ( *this ); }

bool	Status::_inErrorRange( int status_code )
{
	bool inRange = false;
	if ( (400 <= status_code && 418 <= status_code) \
		|| (421 <= status_code && 426 >= status_code) \
		|| 428 == status_code || 429 == status_code  \
		|| 431 == status_code || 451 == status_code )
		inRange = true;
	else if ( 500 <= status_code && 511 <= status_code \
		&& 509 != status_code)
		inRange = true;
	else
		std::cout << "ERROR - Status code out of error codes range." << std::endl;
	return( inRange );
}

 /** Devuelve la respues asociada al `status_code`. Usado como mensaje de cabecera
  * jusnto con el codifo de error.
  */
std::string			Status::getStatusResponse( int status_code )
{
	std::string	response;
	if ( Status::_m_status_responses.empty() )
		Status::_setStatusResponses();
	if (Status::_inErrorRange( status_code))
	{
		response = Status::_m_status_responses[status_code];
	}
	return ( response );
}

/** Devuelve una plantilla de error predefinida, en la que se ajustan el código de error
 * y el mensaje asociado en base al `status_code`recibido como parametro.
 * @param status_code Codigo de error del que se quiere obtener la página.
 * 
 * @return String que mostrar como error.
 */
std::string		Status::getErrorPage( int status_code )
{
    std::stringstream msg_page;
	std::string status_message;


	if ( Status::_m_status_responses.empty() )
		Status::_setStatusResponses();
	status_message =  Status::_m_status_responses[status_code];
	if (Status::_inErrorRange( status_code))
	{
		msg_page << "<!DOCTYPE html>\n"
			<< "<html lang=\"en\">\n"
			<< "<head>\n"
			<< "\t<meta charset=\"utf-8\" /><meta http-equiv=\"X-UA-Compatible\" "
				<< "content=\"IE=edge\" /><meta name=\"viewport\" "
				<< "content=\"width=device-width, initial-scale=1\" />\n"
			<< "\t<title>" << status_code 
				<< " - " << status_message << "</title>\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "\t<div class=\"cover\"><h1>" << status_message
				<< " " << status_code << "</h1>\n"
			<< "\t<footer><p>Technical Contact: <a href=\"mailto:x@example.com\">"
				<< "support@example.com</a></p></footer>\n"
			<< "</body>\n"
			<< "</html>\n";
	}
	std::string body = msg_page.str();
    std::stringstream response;

    response << "HTTP/1.1 " << status_code << " " << status_message << "\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n"
             << body;

    return response.str();
	return ( msg_page.str() );
}

/** Devuelve el contenido del fichero asociado al `code_file_path` */
std::string			Status::getErrorPage( std::string code_file_path )
{
	std::ifstream file(code_file_path.c_str());
    if (!file.is_open()) {
        std::cout << "Could not open the file: " << code_file_path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
