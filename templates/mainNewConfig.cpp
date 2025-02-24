/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainTestConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:53:09 by pabad-ap          #+#    #+#             */
/*   Updated: 2025/02/13 16:53:11 by pabad-ap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Location.hpp"
#include "Status.hpp"

int main ( void )
{
    std::vector<Location *> locations;
    Server server;
    Server server2;

    server.addServerName("localhost")\
        .addServerName("127.0.0.1")\
        .addListen("8080")\
        .addListen("127.0.0.1:80")\
        .addStatusPage(400, "/path/to/page/from/server")\
        .addMRedirection("/fromser", "/to")\
        .addMRedirection("/fromser2", "/to2")\
        .setAutoindex(true)\
        .setIndex("custom_index.html")\
        .setCgi("SERVER_CGI")\
        .setRoot("SERVER_ROOT");

    Location *a = new Location(server, "/");
    a->addServerName("127.0.0.a")
        .addListen("8080a")
        .addListen("127.0.0.1:80a")
        .addStatusPage(400, "/path/to/page/from/serverAAA")
        .addMRedirection("/fromser", "/toAAA")
        .addMRedirection("/fromser2", "/to2AAA")
        .setAutoindex(false)
        .setIndex("custom_indexAAA.html")
        .setCgi("SERVER_CGI")
        .setRoot("SERVER_ROOT");
    locations.push_back(a);

    a = new Location(server, "/www/");
    a->addServerName("127.0.0.a2")
        .addListen("8080a2")
        .addListen("127.0.0.1:80a2")
        .addStatusPage(400, "/path/to/page/from/serverAAA2")
        .addMRedirection("/fromser", "/toAAA2222")
        .addMRedirection("/fromser2", "/to2AAA2222")
        .setAutoindex(true)
        .setIndex("custom_indexAAA.html2")
        .setCgi("SERVER_CGI22")
        .setRoot("SERVER_ROOT22");
    locations.push_back(a);
/**
    server2.addServerName("localhost");
    server2.addServerName("127.0.0.2");
    server2.addListen("4250");
    server2.addListen("127.0.0.2:80");
    server2.addStatusPage(400, "/path/to/page/from/server");
    server2.addMRedirection("/fromserX", "/tox");
    server2.addMRedirection("/fromserXX", "/toxx");
    server2.setAutoindex(false);
    server2.setIndex("2_custom_index.html");
    server2.setCgi("2_SERVER_CGI");
    server2.setRoot("2_SERVER_ROOT");

    location.addStatusPage(300, "/path/to/page/from/location");
    location.addVMethod("GET");
    location.addVMethod("POST");
    location.addVMethod("DELETE");
    location.addMRedirection("/fromloc", "/to");
    location.addMRedirection("/fromloc2", "/to2");
    location.setCgi("LOCATION_CGI");
    location.setRoot("LOCATION_ROOT");

    location2.addStatusPage(300, "/path/to/page/from/location2");
    location2.addVMethod("GET2");
    location2.addVMethod("POST2");
    location2.addVMethod("DELETE2");
    location2.addMRedirection("/fromloc22", "/to22");
    location2.addMRedirection("/fromloc222", "/to222");
    location2.setCgi("LOCATION_CGI2");
    location2.setRoot("LOCATION_ROOT2");
*/
    for ( std::vector<Location *>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        std::cout << **it << std::endl;
        delete *it;
    }
    locations.clear();
    return (0);

}
