#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Content-Type: text/html\r\n\r\n");
    printf("<html><body><h1>CGI en C funcionando!</h1>");
    printf("<p>QUERY_STRING: %s</p>", getenv("QUERY_STRING"));
    printf("</body></html>");
    return 0;
}
