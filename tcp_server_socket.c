#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{
    /*
    NAME
        socket - create an endpoint for communication

    LIBRARY
        Standard C library (libc, -lc)

    SYNOPSIS
        #include <sys/socket.h>

        int socket(int domain, int type, int protocol);
    */
    int sockfd, status;

    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if (sockfd <0)
    {
        perror("socket");
        return sockfd;
    }

    /*
    NAME
     connect - initiate a connection on a socket

    LIBRARY
     Standard C library (libc, -lc)

    SYNOPSIS
     #include <sys/socket.h>

     int connect(int sockfd, const struct sockaddr *addr,
                 socklen_t addrlen);
    */

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    // For server i will allow connection from any ip address
    addr.sin_addr.s_addr = INADDR_ANY;

    /*
    NAME
     bind - bind a name to a socket

    LIBRARY
     Standard C library (libc, -lc)

    SYNOPSIS
     #include <sys/socket.h>

     int bind(int sockfd, const struct sockaddr *addr,
              socklen_t addrlen);
    */

    status = bind(sockfd, (struct sockaddr*)& addr, sizeof(addr));
    if(status)
    {
        perror("bind");
        close(sockfd);
        return status;
    }

    /*
    NAME
     listen - listen for connections on a socket

    LIBRARY
     Standard C library (libc, -lc)

    SYNOPSIS
     #include <sys/socket.h>

     int listen(int sockfd, int backlog);
    */

    status = listen(sockfd, 1);
    if(status)
    {
        perror("listen");
        close(sockfd);
        return status;
    }

    /*
    NAME
     accept, accept4 - accept a connection on a socket

    LIBRARY
     Standard C library (libc, -lc)

    SYNOPSIS
     #include <sys/socket.h>

     int accept(int sockfd, struct sockaddr *_Nullable restrict addr,
                socklen_t *_Nullable restrict addrlen);

     #define _GNU_SOURCE             See feature_test_macros(7)
     #include <sys/socket.h>

     int accept4(int sockfd, struct sockaddr *_Nullable restrict addr,
                socklen_t *_Nullable restrict addrlen, int flags);

    */

    int clientfd;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

    if(clientfd < 0)
    {
        perror("accept");
        close(sockfd);
        return clientfd;
    }

    printf("client port : %d , client ip address %s\n", client_addr.sin_port, inet_ntoa(client_addr.sin_addr));
    /*
    NAME
     send, sendto, sendmsg - send a message on a socket

    LIBRARY
     Standard C library (libc, -lc)

    SYNOPSIS
     #include <sys/socket.h>

     ssize_t send(size_t size;
                    int sockfd, const void buf[size], size_t size, int flags);
     ssize_t sendto(size_t size;
                    int sockfd, const void buf[size], size_t size, int flags,
                    const struct sockaddr *dest_addr, socklen_t addrlen);
     ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);

    */
        
    char text[255];
    int ret = 0;
    ret = recv(clientfd, text, sizeof(text),0);
    if(ret > 0) printf("%s", text);


    sprintf(text, "hello from Server \n");
    send(clientfd, text, strlen(text), 0);

    close(clientfd);
    close(sockfd);
    return 0;

}