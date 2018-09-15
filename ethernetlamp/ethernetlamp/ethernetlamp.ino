//#define dhtpin A1 //Constante
#include <SPI.h>
#include <Ethernet.h>  // Biblioteca utilizada para comunicação com o Arduino
//dht DHT; 
// A linha abaixo permite definir o endereço físico (MAC ADDRESS) da placa de rede. 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

byte ip[] = { 192, 168, 15, 47 };  //Define o endereço IP.  
EthernetServer server(7523);  // Porta onde estará aberta para comunicação Internet e Arduino.
int x = 0;
String readString;
int Pin = 9;  //  Pino digital onde será ligado e desligado a lampada.
int Pin2 = 8; // Pino digital onde sera ligado um segundo aparelho.
//char userPass[200];
//char header[500];
String header;
void setup(){

  Serial.begin(9600);
  pinMode(Pin, OUTPUT);  // Define o Pino 9 como saída.
  pinMode(Pin2, OUTPUT);// Define o Pino 8 como saída.
  Ethernet.begin(mac, ip);  // Chama o MAC e o endereço IP da placa Ethernet.
  server.begin();  //  Inicia o servidor que esta inserido junto a placa Ethernet.
}
 
void loop(){

if(x == 0){
 login(); // Chama a função ethernet.
}
else if (x == 1){
  ethernet();
  }
/*else{
  Serial.println("Offline");
  delay(3000);
  }*/
}

void login(){
   EthernetClient client = server.available();//checa se o cliente de ethernet esta ativo
  if (client) {
     // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
  
        if (c == '\n' && currentLineIsBlank) {

          //parse headers
          //YWRtaW46NjUzMw== = 'admin:6533' (user:password) base64 encode
    
          Serial.print(header);
          
          // Simpler just to find the credential string
          // send a standard http response header
          if(header.indexOf("YWRtaW46NjUzMw==") >= 0) {
            //successful login
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println("Refresh: 5");  // refresh the page automatically every 5 sec
            client.println();
              if(header.indexOf("GET / HTTP/1.1") >= 0) {
                client.println("<!DOCTYPE HTML>");
                client.println("<html>");
                ethernet();
                client.println("</html>");
              } else {
                client.println("<!DOCTYPE HTML>");
                client.println("<html>");
                x=1;
                client.println("Seja Bem vindo");
                client.println("</html>");
              }
            
          } else {
            
            // wrong user/pass
            //client.println("HTTP/1.0 401 Authorization Required");
            client.println("HTTP/1.1 401 Unauthorized");
            client.println("WWW-Authenticate: Basic realm=\"Secure\"");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<html>Text to send if user hits Cancel button</html>"); // really need this for the popup!

          }
          
          header = "";
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  
  }
  }
}

void ethernet(){//função ethernet
//  DHT.read11(dhtpin);//le a entrada de dados do sensor de Temperartura.
 EthernetClient client = server.available();//checa se o cliente de ethernet esta ativo
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
 
        if (readString.length() < 100) {
          readString += c;             
        }

        if (c == '\n') {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Refresh: 8");
          client.println();
    
           // A partir daqui começa os códigos html.

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<title> Projeto Arduino / Project Arduino </title>");
          client.println("<script type=\"text/javascript\" src=\"http://mgcriacoes.com.br/ard/jquery-3.3.1.min.js\"></script>");
          client.println("<script type=\"text/javascript\" src=\"http://mgcriacoes.com.br/ard/arduino.js\"></script>");
          client.println("<link rel=\"stylesheet\" type=\"text/css\" href=\"http://mgcriacoes.com.br/ard/arduino.css\">");
          client.println("</HEAD>");
          client.println("<body bgcolor=\"#ce3333\">");
          client.println("<div align=\"center\" class=\"hold\" >");
         client.println("<div align=\"center\">");
         client.println("<img src=\"https://i.imgur.com/mh3S83I.png\" class=\"img\">");
         client.println("</div>");
       client.println("</div>");
         client.println("<h1 align=\"center\">Sistema de automação</h1>");
         client.println("<h2 align=\"center\">Versão 2.7</h2>");
        
           client.println("<div class=\"menu\">");
             client.println("<div class=\"icone\"> ");
                 client.println("<div align=\"center\" style=\"margin-top:5%;\"><img src=\"https://cdn2.iconfinder.com/data/icons/education-1-35/48/40-128.png\" width=\"40\"></div>");
               client.println("<input type=\"submit\" value=\"Ligar(on) \" onclick=\"location.href='/facacomarduino/LampadaOn'\" class=\"button\"> ");
               client.println("<br>");
               client.println("<input type=\"submit\" value=\"Desligar(off) \" onclick=\"location.href='/facacomarduino/LampadaOff'\" class=\"button\">");
             client.println("</div>");
           client.println("<div class=\"icone\">");
               client.println("<div align=\"center\" style=\"margin-top:5%;\"><img src=\"https://cdn2.iconfinder.com/data/icons/education-1-35/48/40-128.png\" width=\"40\"></div>");
               client.println("<input type=\"submit\" value=\"Ligar(on) \" onclick=\"location.href='/facacomarduino/LampadaOn'\" class=\"button\"> ");
               client.println("<br>");
               client.println("<input type=\"submit\" value=\"Desligar(off) \" onclick=\"location.href='/facacomarduino/LampadaOff'\" class=\"button\">");
           client.println("</div>");
       client.println("</div>");
          client.println("</body>");
          client.println("</HTML>");
           // give the web browser time to receive the data
          delay(1);
          // close the connection:
          client.stop();

          if(readString.indexOf("facacomarduino/LampadaOn") > 0){
            digitalWrite(Pin, HIGH);  // Liga Lampada.
          }
//          else if(readString.indexOf("facacomarduino/Lampada2On") > 0){
//            digitalWrite(Pin2, HIGH); // Liga Lampada 2.
//          }
//          else if(readString.indexOf("facacomarduino/Lampada2Off") > 0){
//            if(readString.indexOf("facacomarduino/Lampada2Off") > 0)
//            {
//              digitalWrite(Pin2, LOW);  // Desliga Lampada 2.
//            }
//          }
          else {
            if(readString.indexOf("facacomarduino/LampadaOff") > 0)
            {
              digitalWrite(Pin, LOW);  // Desliga Lampada
            }
          }
          readString="";    
        }
      }
    }
  }
}

