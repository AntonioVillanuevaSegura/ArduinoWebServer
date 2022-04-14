//Antonio Villanueva Segura Changer l'ip  sauvegarde série dans EEPROM
#ifndef MENU_H_
#define MENU_H_

#include <Arduino.h>

#include <EEPROM.h>   //Uso de memoria EEPROM
//#include <WString.h>

/*
#define MAC 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE
#define IP 192, 168, 6,69
#define PORT 80
*/
//String ip="";
//int ip_mem=0;//Adresse dans la mémoire EEPROM du IP

//Écrire une chaîne dans un emplacement mémoire 
void writeStringToEEPROM(int addrOffset, const String &strToWrite);

//Récupérer une chaîne de la mémoire 
String readStringFromEEPROM(int addrOffset);

//Ecrire une question  retourner la réponse , timeout , block (attendre response)= true false
String readData (String title=" Entrez une nouvelle IP + ENTRÉE  ",int timeout=1000, bool block=true);

//Analyse d'une chaîne IP analyser si la valeur IP est correcte
bool testIp (String ip);

//Afficher un menu simple pour changer ip
void menu(int ip_mem);

#endif //MENU_H_
