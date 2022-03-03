
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

//print the answer page 
void webPage(EthernetClient client,byte value){
    // send a standard http response header
  
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: close"));  // the connection will be closed after completion of the response
  client.println(F("Refresh: 5"));  // refresh the page  every 5 sec
  client.println();
  client.println(F("<!DOCTYPE HTML>"));
  client.println(F("<html>"));
  
  client.println(F("ICARO SERVER on ARDUINO NANO WITH w5500 "));
  client.print(F(" IN VALUE ="));
  
  client.println(value);
  client.println(F("<br />"));
  client.println(F("</html>"));

}

 //There is an active client 
void clientServer(EthernetClient client ,byte in, byte out){
  
    Serial.println(F("new client"));
  // an http request ends with a blank line
  boolean currentLineIsBlank = true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      
      if (c == '\n' && currentLineIsBlank) {
        //webPage(client,value);
        paginaWeb(client,in,out);
          break;
      }
  
      if (c == '\n') {
        // you're starting a new line
        currentLineIsBlank = true;
      } else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
      }
    }
  }
  // give the web browser time to receive the data
  delay(1);
  // close the connection:
  client.stop();
  Serial.println(F("client disconnected"));
}


void creaBotones (EthernetClient client,byte value,String type="_PIN"){
  bool estado=false;


  client.println(F("<br /><br />"));//Salto de linea

  for (uint8_t pin=0;pin<N_SALIDAS;pin++){//Crea los elementos para Encender/Apagar los LEDS con los numeros
    
    //Estado de este pin  segun byte  , out 0xHL
    if (pin && value !=0){estado=true;}
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
  client.println();
  
  //Página web en formato HTML
  client.println(F("<html>"));
  client.println(F("<head>"));
  client.println(F("</head>"));//html
  client.println(F("<body>"));//Body
  client.println(F("<h1 align='center'>ICARO</h1><h2 align='center'>Servidor web control salidas</h2>"));


  client.println(F("<div style='text-align:center;'>"));//Alineacion de botones o grupos 
  //Crea botones .parametros se envian a traves de ? location.href

  creaBotones (client,out,"_POU");
  creaBotones (client,in,"_PIN");
  
  client.println(F("</b></body>"));//Body
  client.println(F("</html>"));//html
  client.println();//FIN HTTP 
  
//  break;
}
