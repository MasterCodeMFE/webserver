    /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   findLocation.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:32:26 by manufern          #+#    #+#             */
/*   Updated: 2025/03/10 19:01:39 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeployServer.hpp"
#include "Request.hpp"

std::string resolveHostnameToIP(const std::string &hostname);

std::string extractExtension(const std::string &filename)
{
    std::size_t lastDotPos = filename.find_last_of('.');
    if (lastDotPos != std::string::npos && lastDotPos != 0)
    {
        return filename.substr(lastDotPos);
    }

    return "";
}

std::string extractFile(const std::string &filename)
{
    std::size_t lastDotPos = filename.find_last_of('/');
    if (lastDotPos != std::string::npos && filename.size() > 1)
    {
        return filename.substr(lastDotPos + 1);
    }
    return "";
}


Location findLocation(const HttpRequest &httpRequest, std::vector<Location> locations) {
    std::string path(httpRequest.path);
    std::string host;
     std::string root(httpRequest.path);
    std::string extension;
    std::string file;
    int i = 0;
    extension = extractExtension(path);
    file = extractFile(path);
    try
    {
        host = httpRequest.headers.at("Host");
    }
    catch(const std::out_of_range& e)
    {
        host = "127.0.0.1:8080";
    }
    if ( path[0] != '/' )
	    path = "/" + path;
    std::vector<std::string> pathSegments;
    pathSegments.push_back(path);
    std::vector<Location>::const_iterator matchLocation = locations.end();
    for (std::vector<Location>::const_iterator locIt = locations.begin(); locIt != locations.end(); ++locIt)
    {
        std::string listen = host;
        
        if (!host.empty() && isspace(host[host.size() - 1])) 
            listen = host.substr(0, host.size() - 1);

        if (i < 1 && extension.compare(locIt->getPath()) == 0  && listen == locIt->getListen())
        {
            matchLocation = locIt;
        }
        if (file.compare(locIt->getPath()) == 0 && listen == locIt->getListen())
        {
            matchLocation = locIt;
            i = 1;
        }
        if (path.compare(locIt->getPath()) == 0 && listen == locIt->getListen())
        {
            matchLocation = locIt;
            break;
        }
    }
    if (matchLocation != locations.end())
    {
        return *matchLocation;
    }
    while ( !path.empty() && path != "/") {
        std::size_t pos = path.find_last_of('/');
        if (pos == 0) {
            path = "/";
        } else {
            path = path.substr(0, pos);
        }
        pathSegments.push_back(path);
    }
    for (std::vector<std::string>::const_iterator segmentIt = pathSegments.begin(); segmentIt != pathSegments.end(); ++segmentIt)
    {   
        for (std::vector<Location>::const_iterator locIt = locations.begin(); locIt != locations.end(); ++locIt)
        {
            std::string listen = host;
            
            if (!host.empty() && isspace(host[host.size() - 1])) 
                listen = host.substr(0, host.size() - 1);

            if (*segmentIt == locIt->getPath() && listen == locIt->getListen())
            {
                return *locIt;
            }
            if (*segmentIt + "/" == locIt->getPath() && listen == locIt->getListen())
            {
                return *locIt;
            }
        }
    }


    std::string domain = host.substr(0, host.find(':'));
    std::string resolvedIP = resolveHostnameToIP(domain);
    if (!resolvedIP.empty())
    {
        host = resolvedIP + host.substr(host.find(':'));
    }
    if ( path[0] != '/' )
	    path = "/" + path;
    matchLocation = locations.end();
    for (std::vector<Location>::const_iterator locIt = locations.begin(); locIt != locations.end(); ++locIt)
    {
        std::string listen = host;
        if (!host.empty() && isspace(host[host.size() - 1])) 
            listen = host.substr(0, host.size() - 1);
        if (i < 1 && extension.compare(locIt->getPath()) == 0  && listen == locIt->getListen())
        {
            matchLocation = locIt;
        }
        if (file.compare(locIt->getPath()) == 0 && listen == locIt->getListen())
        {
            matchLocation = locIt;
            i = 1;
        }
        if (root.compare(locIt->getPath()) == 0 && listen == locIt->getListen())
        {
            matchLocation = locIt;
            break;
        }
    }
    if (matchLocation != locations.end())
    {
        return *matchLocation;
    }

    for (std::vector<std::string>::const_iterator segmentIt = pathSegments.begin(); segmentIt != pathSegments.end(); ++segmentIt)
    {   
        for (std::vector<Location>::const_iterator locIt = locations.begin(); locIt != locations.end(); ++locIt)
        {
            std::string listen = host;
            if (!host.empty() && isspace(host[host.size() - 1])) 
                listen = host.substr(0, host.size() - 1);
                
            if (*segmentIt == locIt->getPath() && listen == locIt->getListen())
            {
                return *locIt;
            }
            if (*segmentIt + "/" == locIt->getPath() && listen == locIt->getListen())
            {
                return *locIt;
            }
        }
    }
    return Location();
}

std::string resolveHostnameToIP(const std::string &hostname) {
    struct addrinfo hints;
    struct addrinfo *res;
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    if (getaddrinfo(hostname.c_str(), NULL, &hints, &res) != 0) {
        std::cerr << "Error resolviendo hostname: " << hostname << std::endl;
        return hostname;
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    unsigned char *ip = (unsigned char *)&(ipv4->sin_addr);
    std::ostringstream ipStr;
    ipStr << (int)ip[0] << "." << (int)ip[1] << "." << (int)ip[2] << "." << (int)ip[3];

    freeaddrinfo(res);

    return ipStr.str();
}

