Here’s a refactored version that combines the temperature sensor code with the network socket code, incorporating both MQTT for messaging and TCP for direct sensor checks.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mosquitto.h>

#define DHT11_PIN 7
#define SERVER_HOST "159.20.20.00/255" 
#define SERVER_PORT 5000

int readDHT11(int fd, int *data) {
    data[2] = wiringPiI2CReadReg8(fd, 0); 
    data[3] = wiringPiI2CReadReg8(fd, 1);  

    if (data[2] == -1 || data[3] == -1) {
        return -1; 
    }
    return 0; 
}

int install_censer() {
    printf("Installing IoT DHT11 sensor\n");
   
    printf("Installation complete\n");
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
        close(sockfd);
        return -1;
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(SERVER_PORT);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        close(sockfd);
        return -1;
    }

    n = write(sockfd, "check_sensor", strlen("check_sensor") + 1);
    if (n < 0) {
        perror("ERROR writing to socket");
        close(sockfd);
        return -1;
    }

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
        perror("ERROR reading from socket");
        close(sockfd);
        return -1;
    }

    if (strcmp(buffer, "sensor_working") == 0) {
        printf("IoT sensor is working.\n");
    } else {
        printf("IoT sensor is not working.\n");
    }

    close(sockfd);
    return 0;
}

int main() {
    wiringPiSetupGpio();

    int dht11_fd = wiringPiI2CSetup(0x40);
    struct mosquitto *mosq;
    mosquitto_lib_init();
    mosq = mosquitto_new("temperature_sensor", true, NULL);

    if (mosq) {
        mosquitto_connect(mosq, "broker.hivemq.com", 1883, 60);
    } else {
        fprintf(stderr, "Failed to initialize MQTT\n");
        return -1;
    }

    install_censer();

    while (1) {
        int data[5] = {0};
        if (readDHT11(dht11_fd, data) == 0) {
            float temperature = data[2] + data[3] / 10.0;
            printf("Temperature: %.2f degrees Celsius\n", temperature);

            char temperature_str[10];
            sprintf(temperature_str, "%.2f", temperature);
            mosquitto_publish(mosq, NULL, "temperature", strlen(temperature_str), temperature_str, 0);

         /* Check sensors status of both DHTC11 and the fire sensor*/
            if (check_censer_status() == -1) {
                fprintf(stderr, "Failed to check sensor status\n");
            }
        } else {
            fprintf(stderr, "Failed to read temperature data\n");
        }
        delay(1000);
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}

