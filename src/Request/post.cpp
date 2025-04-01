/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 20:09:55 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 20:09:57 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Request.hpp"

static int save_uploaded_file(HttpRequest httpRequest, Location location);
static std::string extract_filename(const std::string& body);

    std::string Request::handle_post(const HttpRequest& httpRequest, Location location)
    {
        int error = 0;
        std::stringstream ss;
        unsigned long contentLength = 0;
        
        try {
            std::map<std::string, std::string>::const_iterator it = httpRequest.headers.find("Content-Length");
            if (it != httpRequest.headers.end()) {
                ss << it->second;
                ss >> contentLength;
            } else if ((it = httpRequest.headers.find("Transfer-Encoding")) != httpRequest.headers.end() && it->second == "chunked") {
                return location.getErrorPage(501);
            } else {
                return location.getErrorPage(400);
            }
        
            if (contentLength > location.getClienteMaxBodySize()) {
                return location.getErrorPage(413);
            }
        
            std::string content_type;
            it = httpRequest.headers.find("Content-Type");
            if (it != httpRequest.headers.end()) {
                content_type = it->second;
            } else {
                return location.getErrorPage(400);
            }
        
            std::string response_body;
        
            if (content_type.find("multipart/form-data") != std::string::npos) {
                size_t pos = content_type.find("boundary=");
                if (pos == std::string::npos) {
                    return location.getErrorPage(400);
                }
        
                std::string boundary = "--" + content_type.substr(pos + 9);
                if (boundary.empty() || boundary == "--") {
                    return location.getErrorPage(400);
                }
        
                error = save_uploaded_file(httpRequest, location);
                if (error != 201)
                {
                    return location.getErrorPage(error);
                }
                else
                {
                    response_body = "HTTP/1.1 201 Created\r\n";
                }
            } else {
                response_body = "POST recibido con éxito\nDatos:\n" + httpRequest.body;
            }
        
            return build_http_response(response_body, "text/plain", 201);
        }
        catch (const std::exception& e) {
            return location.getErrorPage(508);
        }
    }

    std::string extract_filename(const std::string& body)
    {   
        size_t pos = body.find("filename=\"");
        if (pos == std::string::npos)
            return "";
        pos += 10;
        size_t end = body.find("\"", pos);
        if (end == std::string::npos)
            return "archivoa";

        std::string filename = body.substr(pos, end - pos);

        while (!filename.empty() && (filename[0] == ' ' || filename[0] == '\t'))
            filename.erase(0, 1);
        while (!filename.empty() && (filename[filename.length() - 1] == ' ' || filename[filename.length() - 1] == '\t'))
            filename.erase(filename.length() - 1, 1);
        if (filename.empty())
            return "archivob";
        if (filename == ".")
            return "archivoc";

        return filename;
    }

    size_t str_to_size_t(const std::string& str)
    {
        std::istringstream iss(str);
        size_t result;
        iss >> result;
        return result;
    }
    std::string int_to_string(int num)
    {
        std::ostringstream oss;
        oss << num;
        return oss.str();
    }

    std::string get_unique_filename(const std::string& directory, const std::string& filename)
    {
        std::string base_name = filename;
        std::string extension = "";
        size_t dot_pos = filename.rfind('.');
        if (dot_pos != std::string::npos && dot_pos != 0) {
            base_name = filename.substr(0, dot_pos);
            extension = filename.substr(dot_pos);
        }

        std::string new_filename = filename;
        int count = 1;
        struct stat buffer;
        while (stat((directory + new_filename).c_str(), &buffer) == 0) {
            std::ostringstream oss;
            oss << base_name << "_" << count << extension;
            new_filename = oss.str();
            count++;
        }

        return new_filename;
    }

std::string remove_multipart_headers(const std::string& body, const std::string& boundary) {

    size_t header_end_len = 4;
    size_t body_len = body.size();
    size_t boundary_len = boundary.size();
    size_t i = 0;
    while (i + header_end_len <= body_len) {
        if (body[i] == '\r' && body[i + 1] == '\n' &&
            body[i + 2] == '\r' && body[i + 3] == '\n') {
            i += header_end_len;
            break;
        }
        ++i;
    }
    if (i >= body_len) return "";
    size_t content_start = i;
    size_t j = content_start;
    while (j + boundary_len <= body_len) {
        size_t k = 0;
        while (k < boundary_len && body[j + k] == boundary[k]) {
            ++k;
        }
        if (k == boundary_len) {
            while (j > content_start && (body[j - 1] == '\r' || body[j - 1] == '\n')) {
                --j;
            }
            break;
        }
        ++j;
    }
    std::string result = body.substr(content_start, j - content_start - 6);
    return result;
}

int save_uploaded_file(HttpRequest httpRequest, Location location)
{
    size_t content_length = 0;
    std::map<std::string, std::string>::iterator it = httpRequest.headers.find("Content-Length");
    if (it != httpRequest.headers.end()) {
        std::istringstream iss(it->second);
        iss >> content_length;
    } else {
        return (411);
    }

    if (location.getClienteMaxBodySize() < content_length) {
        return (413);
    }

    const std::string& body = httpRequest.body;
    std::string filename = extract_filename(body);
    if (filename.empty()) {
        return (503);
    }
    std::string content_type;
    it = httpRequest.headers.find("Content-Type");
    if (it == httpRequest.headers.end()) {
        return (400);
    }
    size_t pos = it->second.find("boundary=");
    if (pos == std::string::npos) {
        return (400);
    }
    std::string boundary = "--" + it->second.substr(pos + 9);
    std::string cleaned_body = remove_multipart_headers(body, boundary);

    std::string upload_dir = location.getRoot() + "/upload/";
    struct stat st;
    if (stat(upload_dir.c_str(), &st) != 0) {
        if (mkdir(upload_dir.c_str(), 0777) != 0) {
            return (500);
        }
    }

    filename = get_unique_filename(upload_dir, filename);
    std::string file_path = upload_dir + filename;

    std::ofstream outfile(file_path.c_str(), std::ios::binary);
    if (!outfile) {
        return (500);
    }
    outfile.write(cleaned_body.c_str(), cleaned_body.size());
    outfile.close();

    return (201);
}