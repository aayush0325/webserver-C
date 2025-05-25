#include <stdio.h> // I/O operations
#include <stdlib.h> // standard library functions
#include <unistd.h> // read()
#include <sys/socket.h> // socket syscalls
#include <string.h> // string handling
#include <netinet/in.h> // socket structs IPv4

#define PORT 3000

int main( int argc, char* argv[] ) {
    char buffer[ 1024 ] = { 0 };
    struct sockaddr_in address;
    ssize_t bytes_read;
    int liseten_fd;
    char *response;
    int accept_fd;
    int socket_fd;
    int bind_fd;

    socket_fd = socket( AF_INET, SOCK_STREAM, 0 );

    if ( socket_fd == 0 ) {
        perror( "Unable to create socket" );
        exit( EXIT_FAILURE );
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    bind_fd = bind( socket_fd, &address, sizeof( address ) );

    if ( bind_fd < 0 ) {
        perror( "Unable to bind to socket" );
        exit( EXIT_FAILURE );
    }

    liseten_fd = listen( socket_fd, 3 );

    if ( liseten_fd < 0 ) {
        perror( "Unable to listen to socket" );
        exit( EXIT_FAILURE );
    }

    printf( "Server running on port %d\n", PORT );

    while( 1 ) {
        accept_fd = accept( socket_fd, &address, sizeof( address ) );

        if ( accept_fd < 0 ) {
            perror( "Failed to accept request" );
        }

        bytes_read = read( accept_fd, buffer, sizeof( buffer ) );

        if ( bytes_read > 0 ) {
            printf( "%s\n", buffer );
        }

        response =  "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: 12\r\n\r\n"
                    "Hello World\n";

        write( accept_fd, response, strlen( response ) );

        close( accept_fd );
    }
}