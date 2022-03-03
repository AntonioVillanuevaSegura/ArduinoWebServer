
#ifndef ETH_H_
#define ETH_H_

#include <SPI.h>
#include <Ethernet.h>
#define N_SALIDAS 8


void ethernetSetup(byte mac,IPAddress ip,EthernetServer server);//Set Up ethernet ARCELI W5500 SPI 
void webPage(EthernetClient client,byte value); //print the answer page  
void clientServer(EthernetClient client,byte in, byte out ); //There is an active client 
void creaBotones (EthernetClient client,byte in,byte out);
void paginaWeb(EthernetClient client,byte in,byte out);
#endif // ETH_H_
