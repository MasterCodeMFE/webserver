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

#include "ACommonConfigs.hpp"
# include "AServerBlockConfigs.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Status.hpp"

int main ( void )
{
    Config config;
    Server server;
    Server server2;
    Location location("/location");
    Location location2("/location2");

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

    server.addServerName("localhost");
    server.addServerName("127.0.0.1");
    server.addListen("8080");
    server.addListen("127.0.0.1:80");
    server.addStatusPage(400, "/path/to/page/from/server");
    server.addVLocation(location);
    server.addVLocation(location2);
    server.addMRedirection("/fromser", "/to");
    server.addMRedirection("/fromser2", "/to2");
    server.setAutoindex(true);
    server.setIndex("custom_index.html");
    server.setCgi("SERVER_CGI");
    server.setRoot("SERVER_ROOT");

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

    config.addStatusPage(200, "/path/to/page/from/config");
    config.addServer(server);
    config.addServer(server2);
    config.setCgi("CONF_CGI");
    config.setRoot("CONF_ROOT");

    std::cout << config << std::endl;

    return (0);
}
