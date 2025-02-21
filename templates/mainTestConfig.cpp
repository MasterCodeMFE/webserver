#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Status.hpp"
#include "./../src/test.hpp"

int main(void)
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

    // --- Configuración del primer servidor ---
    server.addServerName("pepito");
    server.addServerName("127.0.0.1");

    // Se configura para escuchar en la dirección 127.0.0.1, en lugar de 0.0.0.0
    server.addListen("127.0.0.1:8080");   // Escucha en localhost, puerto 8080
    server.addListen("127.0.0.1:80");     // Escucha en localhost, puerto 80

    server.addStatusPage(400, "/path/to/page/from/server");
    server.addVLocation(location);
    server.addVLocation(location2);
    server.addMRedirection("/fromser", "/to");
    server.addMRedirection("/fromser2", "/to2");
    server.setAutoindex(true);
    server.setIndex("custom_index.html");
    server.setCgi("SERVER_CGI");
    server.setRoot("SERVER_ROOT");

    // --- Configuración del segundo servidor ---
    server2.addServerName("juanito");
    server2.addServerName("10.10.1.10");

    // Se configura para escuchar en la dirección 10.10.1.10, en lugar de 0.0.0.0
    server2.addListen("10.10.1.10:4250");  // Escucha en 10.10.1.10, puerto 4250
    server2.addListen("10.10.1.10:80");      // Escucha en 10.10.1.10, puerto 80

    server2.addStatusPage(400, "/path/to/page/from/server");
    server2.addMRedirection("/fromserX", "/tox");
    server2.addMRedirection("/fromserXX", "/toxx");
    server2.setAutoindex(false);
    server2.setIndex("2_custom_index.html");
    server2.setCgi("2_SERVER_CGI");
    server2.setRoot("2_SERVER_ROOT");

    // --- Configuración global ---
    config.addStatusPage(200, "/path/to/page/from/config");
    config.addServer(server);
    config.addServer(server2);
    config.setCgi("CONF_CGI");
    config.setRoot("CONF_ROOT");

    // Muestra la configuración
    std::cout << config << std::endl;

    // Inicia la configuración del servidor
    initialize_server_sockets(config);

    return 0;
}