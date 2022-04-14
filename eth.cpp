//Antonio Villanueva Segura Gestion W5500 spi ethernet
#include "eth.h"


//Set Up ethernet ARCELI W5500 SPI 
void ethernetSetup(byte mac,IPAddress *ip,EthernetServer *server){
  Ethernet.init(10);  // CS D10(Arduino Nano) -->CS W5500
  
  
  Ethernet.begin(mac, *ip); //Start Server web
  
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("Ethernet ARCELI W5500  not found."));
    while (true) {
      delay(1); // Bucle No hardware Error 
    }
  }else { Serial.println(F("Ethernet Hardware W5500 OK "));};
  
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println(F("Ethernet cable is not connected."));
  }
  
  // start the server
  (*server).begin();
  
  Serial.print(F("server IP "));
  Serial.println( Ethernet.localIP());

}

//Make Web buttons OUT /IN
void creaBotones (EthernetClient *client,byte value,String type="_PIN"){
  bool estado=false;

  (*client).println(F("<br /><br />"));//Salto de linea

  for (byte pin=0;pin<N_SALIDAS;pin++){//Crea los elementos para Encender/Apagar los LEDS con los numeros
    
    //Estado de este pin  segun byte  , out 0xHL
    //if (pin && value !=0){estado=true;}
    if (  ( ((0x01) << pin) & value ) !=0){estado=false;} // equivalent( pin && value !=0) 
    else {estado=true;}
    
      if (estado){//Dependiendo del estado de la salida muestra un boton ROJO o VERDE
        (*client).println("<button onClick=location.href=\"/"+String(pin)+type+"\" style='margin:auto;background-color: #24A807;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:200px;height: 80px;'>");
      }else{
        (*client).println("<button onClick=location.href=\"/"+String(pin)+type+"\" style='margin:auto;background-color: #BA0B25;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:200px;height: 80px;'>");
      }
      (*client).println(F("<font size=\"+4\"> "));
      (*client).println( !estado ? "ON  "+String(pin) : "OFF  "+String(pin) );//Texto en boton depende del estado de la salida
      (*client).println(F("</font>"));    
      (*client).println(F("</button>"));

    if ( ((pin+1)& 0x03)==0){//Agrupa botones por grupo de 4 luego salta la linea detecta 4 8 12    equivalent ( ((pin+1)%4)==0)    
      (*client).println(F("<br /><br />"));//Salto de linea
    }

  }

  
}

void paginaWeb(EthernetClient *client,byte in,byte out){
     //Respuesta http al cliente
  //bool estado;//Lee el estado de una salida 
  (*client).println(F("HTTP/1.1 200 OK"));
  (*client).println(F("Content-Type: text/html"));
  //client.println(F(WEB_REFRESH));  // refresh the page  every x sec

  (*client).println();
  //Página web en formato HTML
  (*client).println(F("<html>"));
   //client.println(F("<meta http-equiv="refresh" content="30">"));
  (*client).println(F("<head>"));
  (*client).println (F(" <meta http-equiv=\"refresh\" content=\"1; URL=/\" /> ")) ;// Refresco con envio a la raiz <meta http-equiv="refresh" content="2; URL=/" />
  (*client).println(F("</head>"));//html
  (*client).println(F("<body>"));//Body
  (*client).println(F("<h1 align='center'>ICARO</h1><h2 align='center'>Servidor web control salidas</h2>"));
  (*client).print(F("<h1 align='center'> IN VALUE ="));
  
  (*client).print(in);
  (*client).println(F(" </h1>"));  


  (*client).println(F("<div style='text-align:center;'>"));//Alineacion de botones o grupos 
  //Crea botones .parametros se envian a traves de ? location.href

  creaBotones (client,out,"_POU");
  creaBotones (client,in,"_PIN");
  
  (*client).println(F("</b></body>"));//Body
  (*client).println(F("</html>"));//html
  (*client).println();//FIN HTTP 
  
//  break;
}

byte toggleBit(byte Byte,int pos){
  //Invierte un bit dentro de un byte
   byte value= ( 1<<pos)  & Byte ;//Recupera el bit seleccionado
   byte mask= ~ ( 1<<pos);//Mascara
   byte others= Byte & mask; //Recupera resto de valores
   
   if (value >0){//Es uno 
      value=0;
   }else {value=1<<pos;}


  value |= others;

  return value;//Byte con el bit en position p invertido 
  
}

void clientServer(EthernetClient *client ,DFRobot_MCP23017 *mcp,byte in, byte out){
  String currentLine = "";  
    Serial.println(F("new client"));
  // une requête http se termine par une ligne vide 
  //boolean currentLineIsBlank = true;
  
      while ((*client).connected()) {            // Queda en bucle mientras el cliente este conectado 
      if ((*client).available()) {             // Hay cliente ,datos , bytes para leer del cliente ? 
        char c = (*client).read();             // Entonces leemos un dato un caracter char
        //Serial.write(c);                    // imprime en el puerto serie el dato leido
        if (c == '\n') {                    // ha llegado al final?,fin de linea \n ?
          //Si la linea esta vacia , "blanca", obtenemos dos caractares en una linea
          //Es el final de la solicitud HTTP del cliente , entonces enviamos una respuesta
          
          if (currentLine.length() == 0) {
            
            paginaWeb(client,in,out);// ofrece la pagina WEB al cliente envia estado IN /OUTs

            break;// Salir del bucle que muestra la pagina web
          }else {
            currentLine = "";//Si tenemos nueva linea \n ,limpiamos la linea actual currentLine 
          }
        }
        else if (c != '\r') {    // Si tenemos agun caracter diferente al retorno de carro '\r'
          currentLine += c;      // Escribe el nuevo caracter(char) la linea corriente ,currentLine
        }

        //------------------------------------------------------------------------------------------------------------------------
        // Analizamos la respuesta del cliente en la pagina WEB . Termina con "PIN" , recupera el numero de pin o LED
        //https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Referer

        // give the web browser time to receive the data
        delay(1);

        if ((currentLine.endsWith("POU")  )&& currentLine.indexOf("http")<0){//Busca la palabra clave POW & http
       // if ((currentLine.endsWith("POU") | currentLine.endsWith("PIN") )&& currentLine.indexOf("http")<0){//Busca la palabra clave POW & http
  
            currentLine.remove (currentLine.indexOf('_'));//Busca caracter _ en la linea ,elimina el resto  
            currentLine.remove (0,currentLine.indexOf('/')+1);//Busca caracter / en la linea ,elimina el resto                                          

              out= toggleBit(out,currentLine.toInt());//Invierte bit posicion p en out              
              setPort(mcp,out);
            
            currentLine="";//Limpia la linea .

 
        }
        
        //------------------------------------------------------------------------------------------------------------------------        
      }
      
    } 

  
  // close the connection:
  (*client).stop();

  Serial.println(F("client disconnected"));
}

// Conversion String vers tableau de bytes équivalent ip String="192.168.6.69" --> byte ip[4]={192.168.6.69}
void StringIpToByteIp(String ips ,byte *ip, int size){
    String tmp="";
    for (int i=0; i< ips.length()+1 ;i++){
       if ((ips[i])=='.' || ips[i]==0){ //A atteint le point de séparation ... ou la fin
        *ip=tmp.toInt();// Enregistre la chaîne en tant qu'octet dans le tableau ip
        ip++; //avancer d'une position dans l'array ip de type byte  
        tmp="";//Clear  
       }else {tmp +=(ips[i]);}//avancer d'une position dans le String       
    }
}

//Conversion ip byte array to String ip   byte ip[4]={192.168.6.69} vers --> String="192.168.6.69" 
String ByteIpToString(byte *ip, int size){
  String tmp="";
  while (size --){
    tmp+= String( *(ip++) );
    tmp+= char ((size>0) ? '.': 0);//S'il s'agit du dernier chiffre,  pas de point.
  }
  return tmp;
}
