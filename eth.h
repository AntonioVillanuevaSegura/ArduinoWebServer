//Antonio Villanueva Segura Gestion W5500 spi ethernet
#ifndef ETH_H_
#define ETH_H_

#include <SPI.h>
#include <Ethernet.h>
#include "expander.h" 

#define N_SALIDAS 8
#define WEB_REFRESH "Refresh: 1" //Retard Refre

void ethernetSetup(byte mac,IPAddress *ip,EthernetServer *server);//Set Up ethernet ARCELI W5500 SPI 
void webPage(EthernetClient *client,byte value); //print the answer page  
void clientServer(EthernetClient *client,DFRobot_MCP23017 *mcp,byte in, byte out ); //There is an active client 
void creaBotones (EthernetClient *client,byte in,byte out);
void paginaWeb(EthernetClient *client,byte in,byte out);

// Conversion String vers tableau de bytes équivalent ip String="192.168.6.69" --> byte ip[4]={192.168.6.69}
void StringIpToByteIp(String ips ,byte *ip, int size);

//Conversion ip byte array to String ip   byte ip[4]={192.168.6.69} vers --> String="192.168.6.69" 
String ByteIpToString(byte *ip, int size);

#endif // ETH_H_
