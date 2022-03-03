
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

 // Il y a un client actif 
void clientServer(EthernetClient client ,byte in, byte out){
  String currentLine = "";  
    Serial.println(F("new client"));
  // une requête http se termine par une ligne vide 
  boolean currentLineIsBlank = true;
  
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
      // si vous êtes arrivé à la fin de la ligne (reçu une nouvelle ligne 
      // caractère) et que la ligne est vide, la requête(solicitud) http est terminée, 
      // afin que vous puissiez envoyer une réponse 
      
      if (c == '\n' && currentLineIsBlank) {
        paginaWeb(client,in,out);
          break;
      }
  
      if (c == '\n') {
        // vous démarrez une nouvelle ligne 
        currentLineIsBlank = true;
      } else if (c != '\r') {
        //vous avez un caractère sur la ligne courante 
        currentLineIsBlank = false;
      }

      
    }
  }
  // give the web browser time to receive the data
  delay(1);

  
  //------------------------------------------------------------------------------------------------------------------------
  // Analizamos la respuesta del cliente en la pagina WEB . Termina con "PIN" , recupera el numero de pin o LED
  //https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Referer
  
  //Invierte el estado de una salida
  /*
  if (currentLine.endsWith("PIN")&& currentLine.indexOf("http")<0){//Busca la palabra clave PIN y http
      //Serial.println("LINEA org= "+currentLine); 
      currentLine.remove (currentLine.indexOf('_'));//Busca caracter _ en la linea ,elimina el resto  
      currentLine.remove (0,currentLine.indexOf('/')+1);//Busca caracter / en la linea ,elimina el resto                                          
      digitalWrite( currentLine.toInt(),! digitalRead(currentLine.toInt()));//Invierte el estado 
      currentLine="";//Limpia la linea .
  }
  */
  //------------------------------------------------------------------------------------------------------------------------  

  
  // close the connection:
  client.stop();
  Serial.println(F("client disconnected"));
}

//Make Web buttons OUT /IN
void creaBotones (EthernetClient client,byte value,String type="_PIN"){
  bool estado=false;


  client.println(F("<br /><br />"));//Salto de linea

  for (byte pin=0;pin<N_SALIDAS;pin++){//Crea los elementos para Encender/Apagar los LEDS con los numeros
    
    //Estado de este pin  segun byte  , out 0xHL
    //if (pin && value !=0){estado=true;}
    if (  ( ((0x01) << pin) & value ) !=0){estado=true;}
    else {estado=false;}
    
      if (!estado){//Dependiendo del estado de la salida muestra un boton ROJO o VERDE
        client.println("<button onClick=location.href=\"/"+String(pin)+type+"\" style='margin:auto;background-color: #24A807;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:200px;height: 80px;'>");
      }else{
        client.println("<button onClick=location.href=\"/"+String(pin)+type+"\" style='margin:auto;background-color: #BA0B25;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:200px;height: 80px;'>");
      }
      client.println(F("<font size=\"+4\"> "));
      client.println( !estado ? "ON  "+String(pin) : "OFF  "+String(pin) );//Texto en boton depende del estado de la salida
      client.println(F("</font>"));    
      client.println(F("</button>"));

    //if ( ((pin+1)%4)==0){//Agrupa botones por grupo de 4 luego salta la linea detecta 4 8 12 
    if ( ((pin+1)& 0x03)==0){//Agrupa botones por grupo de 4 luego salta la linea detecta 4 8 12       
      client.println(F("<br /><br />"));//Salto de linea
    }

  }

  
}

void paginaWeb(EthernetClient client,byte in,byte out){
     //Respuesta http al cliente
  bool estado;//Lee el estado de una salida 
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F(WEB_REFRESH));  // refresh the page  every 5 sec
  client.println();
  
  //Página web en formato HTML
  client.println(F("<html>"));
  client.println(F("<head>"));
  client.println(F("</head>"));//html
  client.println(F("<body>"));//Body
  client.println(F("<h1 align='center'>ICARO</h1><h2 align='center'>Servidor web control salidas</h2>"));
  client.print(F("<h1 align='center'> IN VALUE ="));
  
  client.print(in);
  client.println(F(" </h1>"));  


  client.println(F("<div style='text-align:center;'>"));//Alineacion de botones o grupos 
  //Crea botones .parametros se envian a traves de ? location.href

  creaBotones (client,out,"_POU");
  creaBotones (client,in,"_PIN");
  
  client.println(F("</b></body>"));//Body
  client.println(F("</html>"));//html
  client.println();//FIN HTTP 
  
//  break;
}
