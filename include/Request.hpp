/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:14:01 by manufern          #+#    #+#             */
/*   Updated: 2025/03/06 19:14:02 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>
# include <set>

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
#include <poll.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <filesystem>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <sys/stat.h>
#include <dirent.h>
#include <netdb.h>
#include <signal.h>
# include <iterator>


# include "DeployServer.hpp"

class Request
{
	private:
		int tmp; //Atributo aleatorio, pdte. definir atributos reales

		Request( void );
		Request( Request const &src );
		~Request( void );

		Request &operator=( Request const &src );

		//GET
		static std::string	_get_file_path(const std::string& request_path); 
		static std::string	_listDirectory(const std::string &dirPath, const std::string &requestPath);

		//HTTP UTILS
		static std::string	_read_file(const std::string& filepath);
		static std::string	_int_to_string(int number);
		static std::string	_get_content_type(const std::string& filepath);

		//CGI
		static void			_setup_cgi_env(const std::string &script_path, \
								const std::string &query_string, std::vector<char*>& env);

	public:
		static std::string  handle_get(const HttpRequest& request);
		static std::string  handle_post(const HttpRequest& httpRequest);
		static std::string  handle_delete(const HttpRequest& httpRequest);
		static std::string  handle_cgi(const std::string &script_path, const std::string &query_string);
		static std::string	build_http_response(const std::string& content, const std::string& content_type, int status_code);
		static ssize_t		send_all(int sockfd, const char* buffer, size_t length);
};

#endif
