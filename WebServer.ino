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

#define MAC 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE
#define IP 192,168,6,69
#define PORT 80
#define I2C_ADDRESS 0x20 //mcp23017 expander

int ip_mem=0;//Adresse dans la mémoire EEPROM du IP

#include "menu.h"
#include "expander.h" 
#include "eth.h"

//Expander MCP 23017
DFRobot_MCP23017 mcp(Wire, /*addr =*/I2C_ADDRESS);

//W5500 ETHERNET
EthernetServer server(PORT);//PORT 80 http

void setup() {
  byte mac[]={MAC};
  byte ipb[4];//Tableau de 4 bytes pour recevoir la valeur IP String
  String ips="";
  //IPAddress ip(IP);//IP address

  
  Serial.begin(9600); //Serial Debug
  //Serial.setTimeout(5000); //Time Out Serial
  
  while (!Serial) { } //Wait USB Serial ttyUSB
  Serial.println(F("Ethernet WebServer"));

  //Menu avec possibilité de changer d'ip

  int time_menu=3;
  
  while ( (time_menu--) >0){ menu(ip_mem);} //Affiche le menu pour changer l'IP
  
  ips=readStringFromEEPROM(ip_mem);//Read IP from EEPROM
  if ( ! testIp ((String)ips)){//IP is OK ?
    ips=IP;//Si l'EEPROM ne contient pas de valeur correcte attribue une valeur IP par défaut
  }

//void StringIpToByteIp(String ips ,byte *ip, int size);
  StringIpToByteIp(ips,ipb,4);//ip String to ip byte array ips ---to--->ipb
  IPAddress ip(ipb);//IP address from byte array byte ipb[4]={ip1,ip2,ip3,ip4}
  
  expanderSetup ( &mcp );//mcp23017 set Up
  ethernetSetup (mac,&ip,&server);//Ethernet Set Up
}
 
void loop() {
  // listen for incoming clients
  EthernetClient client = server.available(); 

  if (client) {
    clientServer(&client,&mcp,readPort(&mcp,'A') , readPort(&mcp,'B') ); //Lecture PORTA IN
  }
 
}
