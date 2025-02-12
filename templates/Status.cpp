/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:44:28 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/12 17:23:38 by manufern         ###   ########.fr       */
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

Status::Status( Status const &src ): _status_code(src._status_code), \
	_error_page_path(src._error_page_path){}

/** Constructor to set customized error page path.
 * If `_m_status_responses_`is empty it loadas it.
*/
Status::Status( int status_code, std::string error_page_path ): \
	_status_code(status_code), _error_page_path(error_page_path)
{
	if ( Status::_m_status_responses.empty() )
		Status::_setStatusResponses();
}

Status::~Status( void ){}

Status	&Status::operator=( Status const &src )
{
	if ( this != &src )
	{
		this->_status_code = src._status_code;
		this->_error_page_path = src._error_page_path;
	}
	return ( *this );
}

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

 /** Get default response associated to status_code. Used
  * to get it when no custom response is setted.
  */
std::string			Status::getDefaultStatusResponse( int status_code )
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
std::string Status::getDefaultErrorPage(int status_code)
{
    std::stringstream msg_page;

    if (Status::_m_status_responses.empty())
        Status::_setStatusResponses();

    std::string status_message = "Unknown Error";
    if (Status::_m_status_responses.find(status_code) != Status::_m_status_responses.end())
        status_message = Status::_m_status_responses[status_code];

    msg_page << "<!DOCTYPE html>\n"
             << "<html lang=\"en\">\n"
             << "<head>\n"
             << "\t<meta charset=\"utf-8\" />\n"
             << "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n"
             << "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n"
             << "\t<title>" << status_code << " - " << status_message << "</title>\n"
             << "\t<style>\n"
             << "\t\tbody { font-family: Arial, sans-serif; background-color: #f8f8f8; text-align: center; padding: 50px; }\n"
             << "\t\t.container { max-width: 600px; margin: auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1); }\n"
             << "\t\th1 { font-size: 50px; color: #ff6b6b; }\n"
             << "\t\tp { font-size: 18px; color: #333; }\n"
             << "\t\t.footer { margin-top: 20px; font-size: 14px; color: #888; }\n"
             << "\t\t@keyframes fadeIn { from { opacity: 0; } to { opacity: 1; } }\n"
             << "\t\t.cover { animation: fadeIn 1.5s ease-in-out; }\n"
             << "\t</style>\n"
             << "</head>\n"
             << "<body>\n"
             << "\t<div class=\"container cover\">\n"
             << "\t\t<h1>" << status_code << "</h1>\n"
             << "\t\t<p>" << status_message << "</p>\n"
             << "\t</div>\n"
             << "</body>\n"
             << "</html>\n";

    std::string body = msg_page.str();
    std::stringstream response;

    response << "HTTP/1.1 " << status_code << " " << status_message << "\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n"
             << body;

    return response.str();
}


int		Status::getStatusCode( void ) const
{
	return ( this->_status_code );
}

std::string const &Status::getErrorPagePath( void ) const
{
	return ( this->_error_page_path );
}

/** Return text of `_error_page_path` file.*/
std::string			Status::getErrorPage( void ) const
{
	std::ifstream file(this->_error_page_path.c_str());
    if (!file.is_open()) {
        std::cout << "Could not open the file: " << this->_error_page_path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::ostream	&operator<<( std::ostream &o, Status const &src)
{
	o << "error_page " << src.getStatusCode() << " " << src.getErrorPagePath() << ";" << std::endl;
	return ( o );
}
