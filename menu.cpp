//Antonio Villanueva Segura Changer l'ip  sauvegarde série dans EEPROM
#include "menu.h"

//String ip="";
//int ip_mem=0;//Adresse dans la mémoire EEPROM du IP

//String mac="";
//int mac_mem=0;//Adresse dans la mémoire EEPROM du MAC 

//Écrire une chaîne dans un emplacement mémoire 
void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{ 

  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  Serial.print (strToWrite);Serial.println (" write in EEPROM !");//DEBUG
}

//Récupérer une chaîne de la mémoire 
String readStringFromEEPROM(int addrOffset){
  
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  Serial.print (data);Serial.println ("Read from EEPROM ");//DEBUG
  return String(data);
}

//Ecrire une question  retourner la réponse , timeout , block (attendre response)= true false
String readData (String title=" Entrez une nouvelle IP + ENTRÉE  ",int timeout=1000, bool block=true){
  String tmp="";
  Serial.setTimeout(timeout);
  Serial.println (title);  
  //while (tmp==""){tmp=Serial.readString();} //Attendre response
  do{ tmp = Serial.readString() ;} while ( (tmp=="") && block);
  Serial.setTimeout(1000);//Norma
  
  return tmp;  
}

//Analyse d'une chaîne IP analyser si la valeur IP est correcte
bool testIp (String ip){
  int ptos=0;
  if (ip.length() >( 15 +1 )  ){return false;} //ex 255.255.255.255 >
  if (ip.length() <(7 + 1 ) ){return false;}  //ex 0.0.0.0 <

  //compter le nombre de points  ... ex 192.168.3. 3 points
  for (byte index=0; index < ip.length()-1 ;index++){if ( ip[index]=='.') {ptos++;}}

  if (ptos!=3){return false;}//une adresse ip contient 3 points 

  //Ctrl.ascii caractères non autorisés 
  for (int index=0 ; index < ip.length()-1  ; index++ ){
    if ( ( ip[index]<'0'||ip[index] >'9')&& ip[index]!='.') {return false;}}

  //Ctrl. last char number ,le dernier caractère doit être un nombre 
  if ( ip [ip.length()-2]<'0'||ip [ip.length()-2]>'9'){ return false;}

  //Analyse les blocs de nombres entre 0 et 255 aaa.bbb.ccc.ddd
  const char * separator = ".";
  char ipc[ip.length()];
  ip.toCharArray(ipc,ip.length());

  char * strToken = strtok ( ipc, separator );
  while ( strToken != NULL ) {
      //Serial.println (  strToken );
      strToken = strtok ( NULL, separator );// On demande le token suivant.

      //Serial.print ("Version int = ");Serial.println (atoi(strToken));
     if ( atoi(strToken)>255){return false;}
  }
 
  return true;
}

//Afficher un menu simple pour changer ip
void menu(int ip_mem){
    String tmp="";
    tmp = readData ("Change ip (y) or (Y) ?",2000,false);//Change ip Yes or Not ?
    
    if (tmp!="" &&  ( tmp[0]=='y'||tmp[0]=='Y' )){//YES change IP      
      
      tmp = readData (" Entrez une nouvelle IP + ENTRÉE  ");
      Serial.print (" Valeur IP ");Serial.println (tmp);

      if (testIp( tmp )){//Test ip
        writeStringToEEPROM(ip_mem, tmp);
      }      
      tmp="";         
    }  
}
