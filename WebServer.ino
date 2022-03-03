/*
 * Antonio Villanueva Segura WebServer con spi ethernet W5500 y bus expander mcp23017
 https://docs.arduino.cc/tutorials/ethernet-shield-rev2/WebServer

ARDUINO NANO 
A5 SCL Yellow
A4 SDA Green
D10 CS SPI
D11 MOSI SPI
D12 MISO SPI
D13 SCK SPI
 
ARCELI W5500 SPI 
LAN Ethernet Module réseau TCP IP STM32 Interface 3.3V 5V 
https://www.amazon.fr/gp/product/B09DYFLMJG/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1
GND                           GND 
GND                           VCC --> +3V3 arduino nano
MOSI --> D11 arduino nano     VCC --> +3V3 arduino nano
SCK -->  D13 arduino nano     NC
CS  -->  D10 arduino nano     RST
INT                           MISO --> D12 arduino nano 
 */

#include "expander.h" 
#include "eth.h"


#define I2C_ADDRESS 0x20 //mcp23017 expander

//W5500 ETHERNET
#define MAC 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE
#define IP 192, 168, 6,69
#define PORT 80


DFRobot_MCP23017 mcp(Wire, /*addr =*/I2C_ADDRESS);

EthernetServer server(PORT);//PORT 80 http

void setup() {
  byte mac[]={MAC};
  IPAddress ip(IP);//IP address

  Serial.begin(9600); //Serial Debug
  
  while (!Serial) { } //Wait USB Serial ttyUSB
  Serial.println(F("Ethernet WebServer"));
  
  expanderSetup ( &mcp );//mcp23017 set Up
  ethernetSetup (mac,&ip,&server);//Ethernet Set Up
}
 
void loop() {
  // listen for incoming clients
  EthernetClient client = server.available(); 
  
  if (client) {
    clientServer(client,readPort(&mcp,'A') , readPort(&mcp,'B') ); //Lecture PORTA IN
  }
  
}
