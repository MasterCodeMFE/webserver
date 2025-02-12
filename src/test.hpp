#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <poll.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <filesystem>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

#define PORT 8080

class Server;
class Status;

struct HttpRequest
{
    std::string method;
    std::string path;
    std::string protocol;
    std::map<std::string, std::string> headers;
    std::string body;  // 🔹 Cuerpo del mensaje (para POST)
    std::string query_string;  // 🔹 Parámetros en la URL (para CGI y GET)
};


class Config
{
	public:
		std::vector<Server*>		v_servers;
		std::vector<Status*>		m_status_pages;
		int							client_max_body;
		bool						autoindex;
		std::string					index;
		std::string					cgi;
		std::string					root;

		Config( void );
		Config( Config const &src );
		~Config( void );

		Config	&operator=( Config const &src );
};

class Location
{
	public:
		std::vector<Status*>				m_status_pages;
		int									client_max_body;
		std::vector<std::string>			v_methods;	
		std::map<std::string, std::string>	m_redirections;
		bool								autoindex;
		std::string							index;
		std::string							cgi;
		std::string							root;

		Location( void );
		Location( Location const &src );
		~Location( void );

		Location	&operator=( Location const &src );
};


class Server
{
	public:
		std::string							server_name;
		std::vector<std::string>			v_listen;
		std::vector<Status*>				m_status_pages;
		int									client_max_body;
		std::vector<Location *>				m_locations;
		std::map<std::string, std::string>	m_redirections;
		bool								autoindex;
		std::string							index;
		std::string							cgi;
		std::string							root;

		Server( void );
		Server( Server const &src );
		~Server( void );

		Server	&operator=( Server const &src );
};

class Status
{
	public:	
		static std::map<int, std::string>	_m_status_responses;
		int									_status_code;
		std::string							_error_page_path;

		Status( void );
		Status( Status const &src );

		Status	&operator=( Status const &src );
		
		Status( int status_code, std::string error_page_path );
		~Status( void );

		static void							setStatusResponses( void );

		static std::string const 			&getStatusResponse( int status_code );
		static std::string const			&getDefaultErrorPage( int status_code );
		
		int									getStatusCode( void ) const;
		std::string const					&getErrorPagePath( void ) const;
		std::string	const					&getErrorPage( int status_code ) const;
};

int create_socket();
void close_client(int client_fd);
int paso_dos(int server_fd, Config *config);
int paso_tres(int server_fd, Config *config);
int paso_cuatro(int server_fd);
int paso_cinco(int client_fd);
int paso_seis(int client_fd, const HttpRequest& httpRequest);

#endif // TEST_HPP