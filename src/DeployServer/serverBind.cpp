/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBind.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:07:58 by manufern          #+#    #+#             */
/*   Updated: 2025/03/31 19:49:52 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "DeployServer.hpp"


int string_to_int(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    return value;
}


std::pair<std::string, std::string> parseListenString(const std::string& listen_str) {
    std::pair<std::string, std::string> result;
    size_t pos = listen_str.find(':');
    if (pos != std::string::npos) {
        result.first = listen_str.substr(0, pos);  
        result.second = listen_str.substr(pos + 1);
    } else {
        result.first = "";  
        result.second = listen_str;
    }
    return result;
}


struct addrinfo* getResolvedAddress(const std::string& ip, const std::string& port_str) {
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE;   

    struct addrinfo *res;
    int status = getaddrinfo(ip.empty() ? NULL : ip.c_str(), port_str.c_str(), &hints, &res);
    if (status != 0) {
        std::cerr << "Error en getaddrinfo con la IP: " 
                  << (ip.empty() ? "INADDR_ANY" : ip)
                  << ". Details: " << gai_strerror( status ) << std::endl;
        return NULL;
    }
    return res;
}


int attemptBind(int server_fd, struct addrinfo* res, const std::string& ip, int port)
{
    if (bind(server_fd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "Error al vincular el socket en " 
                  << (ip.empty() ? "INADDR_ANY" : ip)
                  << ":" << port << ": " << strerror(errno) << std::endl;
        freeaddrinfo(res);
        
        return -1;
    }
    freeaddrinfo(res);
    return 0;
}


int configure_and_bind_socket(int server_fd, std::pair<std::string, std::string> const &listener) {

    std::string listen_str = listener.first;
    
 
    std::pair<std::string, std::string> listenPair = parseListenString(listen_str);
    std::string ip = listenPair.first;
    std::string port_str = listenPair.second;
    int port = string_to_int(port_str);

    struct addrinfo* res = getResolvedAddress(ip, port_str);
    if (!res) {
        return -1;
    }


    return attemptBind(server_fd, res, ip, port);
}