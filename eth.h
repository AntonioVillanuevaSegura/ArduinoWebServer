//Antonio Villanueva Segura Gestion W5500 spi ethernet
#ifndef ETH_H_
#define ETH_H_

#include <SPI.h>
#include <Ethernet.h>
#define N_SALIDAS 8
#define WEB_REFRESH "Refresh: 1" //Retard Refre

void ethernetSetup(byte mac,IPAddress *ip,EthernetServer *server);//Set Up ethernet ARCELI W5500 SPI 
void webPage(EthernetClient client,byte value); //print the answer page  
void clientServer(EthernetClient client,byte in, byte out ); //There is an active client 
void creaBotones (EthernetClient client,byte in,byte out);
void paginaWeb(EthernetClient client,byte in,byte out);
#endif // ETH_H_
