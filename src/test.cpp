#include "test.hpp"

// Implementación de Config
Config::Config() {}
Config::Config(Config const &src) {*this = src;}
Config::~Config() {}
Config &Config::operator=(Config const &src) {
    if (this != &src) {
        v_servers = src.v_servers;
        m_status_pages = src.m_status_pages;
        client_max_body = src.client_max_body;
        autoindex = src.autoindex;
        index = src.index;
        cgi = src.cgi;
        root = src.root;
    }
    return *this;
}

// Implementación de Server
Server::Server() {}
Server::Server(Server const &src) {*this = src;}
Server::~Server() {}
Server &Server::operator=(Server const &src) {
    if (this != &src) {
        server_name = src.server_name;
        v_listen = src.v_listen;
        m_status_pages = src.m_status_pages;
        client_max_body = src.client_max_body;
        m_locations = src.m_locations;
        m_redirections = src.m_redirections;
        autoindex = src.autoindex;
        index = src.index;
        cgi = src.cgi;
        root = src.root;
    }
    return *this;
}

// Implementación de Location
Location::Location() {}
Location::Location(Location const &src) {*this = src;}
Location::~Location() {}
Location &Location::operator=(Location const &src) {
    if (this != &src) {
        m_status_pages = src.m_status_pages;
        client_max_body = src.client_max_body;
        v_methods = src.v_methods;
        m_redirections = src.m_redirections;
        autoindex = src.autoindex;
        index = src.index;
        cgi = src.cgi;
        root = src.root;
    }
    return *this;
}

// Implementación de Status
Status::Status() : _status_code(0), _error_page_path("") {}
Status::Status(int status_code, std::string error_page_path)
    : _status_code(status_code), _error_page_path(error_page_path) {}
Status::Status(Status const &src) {*this = src;}
Status::~Status() {}
Status &Status::operator=(Status const &src) {
    if (this != &src) {
        _status_code = src._status_code;
        _error_page_path = src._error_page_path;
    }
    return *this;
}