/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:44:28 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/11 15:33:36 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Status.hpp"

/** Variable that store default status codes and their default description */
void	Status::setStatusResponses( void )
{
	if ( Status::_m_status_responses.empty() )
	{
		std::map<int, std::string>	status_map ={	
			//1xx informational response
			{100, "Continue"},
			{101, "Switching Protocols"},
			{102, "Processing"},
			{103, "Early Hints"},
		
			//2xx success
			{200, "OK"},
			{201, "Created"},
			{202, "Accepted"},
			{203, "Non-Authoritative Information (since HTTP/1.1)"},
			{204, "No Content"},
			{205, "Reset Content"},
			{206, "Partial Content"},
			{207, "Multi-Status (WebDAV, RFC 4218)"},
			{208, "Already Reported (WebDAV, RFC 5842)"},
			{226, "IM Used (RFC 3229)"},
		
			//3xx redirection
			{300, "Multiple Choices"},
			{301, "Moved Permanently"},
			{302, "Foundi (Previously \"Moved temporarily\")"},
			{303, "See Other (since HTTP/1.1)"},
			{304, "Not Modified"},
			{305, "Use Proxy (since HTTP/1.1)"},
			{306, "Switch Proxy"},
			{307, "Temporary Redirect (since HTTP/1.1)"},
			{308, "Permanent Redirect"},
			
			//4xx client errors	
			{400, "Bad Request"},
			{401, "Unauthorized"},
			{402, "Payment Required"},
			{403, "Forbidden"},
			{404, "Not Found"},
			{405, "Method Not Allowed"},
			{406, "Not Acceptable"},
			{407, "Proxy Authentication Required"},
			{408, "Request Timeout"},
			{409, "Conflict"},
			{410, "Gone"},
			{411, "Length  Required"},
			{412, "Request Timeout"},
			{413, "Payload Too Large"},
			{414, "URI Too Long"},
			{415, "Unsupported Media Type"},
			{416, "Range Not Satisfiable"},
			{417, "Expectation Failed"},
			{418, "I'm a teapot (RFC 2324, RFC 7168)"},
			{421, "Misdirected Request"},
			{422, "Unprocessable Content"},
			{423, "Locked (WebDAV,RFC 4918)"},
			{424, "Failed Dependency (WebDAV,RFC 4918)"},
			{425, "Too Early (RFC 8470)"},
			{426, "Upgrade Required"},
			{428, "Precondition Required (RFC 6585)"},
			{429, "Too Many Requests (RFC 6585)"},
			{431, "Request Header Field Too Large (RFC 6585)"},
			{451, "Unavailable For Legal Reasons (RFC 7725)"},
		
			//5xx server errors
			{500, "Internal Server Error"},
			{501, "Not Implemented"},
			{502, "Bad Gateway"},
			{503, "Service Unavailable"},
			{504, "Gateway Timeout"},
			{505, "HTTP Version Not Supported"},
			{506, "Variant Also Negotiates"},
			{507, "Insufficient Storage"},
			{508, "Loop Detected"},
			{510, "Not Extended"},
			{511, "Network Authentication Required (RFC 6585)"}
		};
		Status::_m_status_responses =  status_map;
	}
}

Status::Status( void ){}

Status::Status( Status const &src ): _status_code(src._status_code), \
	_error_page_path(src._error_page_path){}

/** Constructor to set customized error page path. */
Status::Status( int status_code, std::string error_page_path ): \
	_status_code(status_code), _error_page_path(error_page_path){}

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

std::string const	&Status::getStatusResponse( int status_code )
{
	return ( Status::_m_status_responses[status_code] );
}

std::string const	&Status::getDefaultErrorPage( int status_code )
{
	std::stringstream	msg_page;

	msg_page << "<!DOCTYPE html>\n"
		<< "<html lang=\"en\">\n"
		<< "<head>\n"
		<< "\t<meta charset=\"utf-8\" /><meta http-equiv=\"X-UA-Compatible\" "
			<< "content=\"IE=edge\" /><meta name=\"viewport\" "
			<< "content=\"width=device-width, initial-scale=1\" />\n"
		<< "\t<title>" << status_code 
			<< " - " << Status::_m_status_responses[status_code] << "</title>\n"
		<< "</head>\n"
		<< "<body>\n"
		<< "\t<div class=\"cover\"><h1>" << Status::_m_status_responses[status_code]
			<< " " << status_code << "</h1>\n"
		<< "\t<footer><p>Technical Contact: <a href=\"mailto:x@example.com\">"
			<< "support@example.com</a></p></footer>\n"
		<< "</body>\n"
		<< "</html>\n";
	
	return ( msg_page.str() );
}

int		Status::getStatusCode( void ) const
{
	return ( this->_status_code );
}

std::string const &Status::getErrorPagePath( void ) const
{
	return ( this->_error_page_path );
}

std::string const	&Status::getErrorPage( int status_code ) const
{
	(void ) status_code;
	return "HOLAAA";
}

std::ostream	&operator<<( std::ostream &o, Status &src)
{
	o << "STATUS CODE: " << src.getStatusCode()
		<< "\nERROR_PAGE_PATH " << src.getErrorPagePath() << std::endl;
	return ( o );
}
