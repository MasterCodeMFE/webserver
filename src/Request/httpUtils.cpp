/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:13 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 20:02:05 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string Request::_read_file(const std::string& filepath)
{
    std::ifstream file(filepath.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string Request::_int_to_string(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}
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
    return "application/octet-stream";
}
std::string Request::build_http_response(const std::string& content, const std::string& content_type, int status_code)
{
    return "HTTP/1.1 " + Request::_int_to_string(status_code) + " " + Status::getStatusResponse(status_code) + "\r\n"
           "Content-Type: " + content_type + "\r\n"
           "Content-Length: " + Request::_int_to_string(content.size()) + "\r\n\r\n" + content;
}
ssize_t Request::send_all(int sockfd, const char* buffer, size_t length)
{
    size_t total_sent = 0;
    while (total_sent < length)
    {
        ssize_t bytes_sent = send(sockfd, buffer + total_sent, length - total_sent, MSG_NOSIGNAL);
        if (bytes_sent <= 0)
        {
            return -1;
        }
        total_sent += bytes_sent;
    }
    return total_sent;
}