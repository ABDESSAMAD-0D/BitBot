#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#define SERVER_HOST "192.168.1.100" // Replace with your server's IP address
#define SERVER_PORT 5000


int install_censer() {
    printf("Installing IoT censer...\n");

    // Replace this with your actual installation steps.

    printf("Installation complete.\n");
    return 0;
}


int check_censer_status() {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return -1;
    }

    
    server = gethostbyname(SERVER_HOST);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host as %s\n", SERVER_HOST);
        return -1;
    }

   
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(SERVER_PORT);

   
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        return -1;
    }

   
    n = write(sockfd, "check_censer", strlen("check_censer") + 1);
    if (n < 0) {
        perror("ERROR writing to socket");
        return -1;
    }

    
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
        perror("ERROR reading from socket");
        return -1;
    }

 
    if (strcmp(buffer, "censer_working") == 0) {
        printf("IoT censer is working.\n");
    } else {
        printf("IoT censer is not working.\n");
    }

   
    close(sockfd);

    return 0;
}

int main() {
    install_censer();
    check_censer_status();
    return 0;
}
