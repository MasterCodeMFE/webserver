#include "./Status.hpp"
#include <iostream>
#include <string>

int main ( void )
{
    Status define( 404, "../errors/error404.html");
    
    std::cout << define << std::endl;
    std::cout << Status::getDefaultStatusResponse( 404 ) << std::endl;
    std::cout << Status::getDefaultErrorPage( 4040 ) << std::endl;
    std::cout << define.getStatusCode() << define.getErrorPage( ) << std::endl;
    return ( 0 );
}