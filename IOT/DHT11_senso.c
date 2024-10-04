#include <stdio.h>
// This library can only be used with a raspberry pi board.
#include <wiringPi.h>
//Same with this one
#include <wiringPiI2C.h>
#include <mosquitto.h>

#define DHT11_PIN 7

int readDHT11(int fd, int *data) {
    data[2] = wiringPiI2CReadReg8(fd, 0); 
    data[3] = wiringPiI2CReadReg8(fd, 1);  

    if (data[2] == -1 || data[3] == -1) {
        return -1; 
    }

    return 0; 
}


int main() {
  wiringPiSetupGpio();

  int dht11_fd = wiringPiI2CSetup(0x40);

  struct mosquitto *mosq;
  mosquitto_lib_init();
  mosq = mosquitto_new("temperature_sensor", true, NULL);
  mosquitto_connect(mosq, "broker.hivemq.com", 1883, 60);

  while (1) {
    int data[5] = {0};
    if (readDHT11(dht11_fd, data) == 0) {
      float temperature = data[2] + data[3] / 10.0;
      printf("Temperature: %.2f degrees Celsius\n", temperature);

      char temperature_str[10];
      sprintf(temperature_str, "%.2f", temperature);
      mosquitto_publish(mosq, NULL, "temperature", strlen(temperature_str), temperature_str, 0);
    }

    delay(1000);
  }

  return 0;
}
