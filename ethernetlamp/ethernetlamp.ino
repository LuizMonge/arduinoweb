 // Projeto 23 – Ascendendo lâmpada com Shield Ethernet e rele
#include <dht.h> //Inclusão da biblioteca
#define dhtpin A1 //Constante
#include <SPI.h>
#include <Ethernet.h>  // Biblioteca utilizada para comunicação com o Arduino
dht DHT; 
// A linha abaixo permite definir o endereço físico (MAC ADDRESS) da placa de rede. 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

byte ip[] = { 192, 168, 1, 47 };  //Define o endereço IP.  
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
  DHT.read11(dhtpin);//le a entrada de dados do sensor de Temperartura.
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
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<body bgcolor=\"#E6E6FA\">");
          client.println("<center><img src=\"https://electrosome.com/wp-content/uploads/2014/06/arduino.png\" alt=\"Smiley face\" height=\"128\" width=\"128\"></center>");
          client.println("<H1><center>Projeto Com Arduino / Project with Arduino V.2.3</center></H1>");
          client.println("<H4><center>Desenvolvido por / Made by: Luiz Henrique Miguel Monge</center></H4>");
          client.println("<hr />");
          client.println("<br />");
          client.println("<H1>Ligar Aparelhos / Turn on appliances</H1>");
          client.println("<H2><font color=\"orange\" >Lampada / Lamp:</font></H2>");
          client.println("<input type=\"submit\" value=\"Desligar(off) \" onclick=\"location.href='/facacomarduino/LampadaOn'\">");
          client.println("<input type=\"submit\" value=\"Ligar(on) \" onclick=\"location.href='/facacomarduino/LampadaOff'\"><br /><br />");
          client.println("<H2><font color=\"orange\" > Aparelho de Som / Sound appliance:</font></H2>");
          client.println("<input type=\"submit\" value=\"Desligar(off)\" onclick=\"location.href='/facacomarduino/Lampada2On'\">");
          client.println("<input type=\"submit\" value=\"Ligar(on)\" onclick=\"location.href='/facacomarduino/Lampada2Off'\"><br /><br />");
          client.println("<H2><font color=\"blue\">temperatura/temperature: </font></H2>");
          client.println(DHT.temperature);
          client.println("<font color=\"green\"> Celsius</font>");
          client.println("<br />");
          client.println("<H2><font color=\"red\">Intrucoes / instructions:</font></H2>");
          client.println("<H3>Para ligar os aparelhos clique no botao Ligar ou Desligar</H3>");
          client.println("<H3>Se a lampada ja estiver acesa basta apenas inverter os comandos, no botão ligar desliga a lampada e o desligar liga a lampada</H3>");
          client.println("<H4>To turn on the appliances click on button turn on or off</H4>");
          client.println("<H4>If the lamp is already activated just invert the commands, on the button ON turns off the lamp and the button OFF turn on the lamp</H4>");
          client.println("<br />");
          client.println("<center><div style=\"margin: 15px 0px 0px; display: inline-block; text-align: center; width: 200px;\"><noscript><div style=\"display: inline-block; padding: 2px 4px; margin: 0px 0px 5px; border: 1px solid rgb(204, 204, 204); text-align: center; background-color: transparent;\"><a href=\"http://localtimes.info/South_America/Brazil/Sao_Paulo/Parapua/\" style=\"text-decoration: none; font-size: 13px; color: rgb(0, 0, 0);\"><img src=\"http://localtimes.info/images/countries/br.png\"="" border=0="" style=\"border:0;margin:0;padding:0\"=""> Parapua</a></div></noscript><script type=\"text/javascript\" src=\"http://localtimes.info/clock.php?continent=South America&country=Brazil&province=Sao Paulo&city=Parapua&cp1_Hex=000000&cp2_Hex=FFFFFF&cp3_Hex=000000&fwdt=200&ham=1&hbg=1&hfg=1&sid=0&mon=0&wek=0&wkf=0&sep=0&widget_number=1021\"></script></div></center>");
          client.println("</HTML>");
           // give the web browser time to receive the data
          delay(1);
          // close the connection:
          client.stop();

          if(readString.indexOf("facacomarduino/LampadaOn") > 0){
            digitalWrite(Pin, HIGH);  // Liga Lampada.
          }
          else if(readString.indexOf("facacomarduino/Lampada2On") > 0){
            digitalWrite(Pin2, HIGH); // Liga Lampada 2.
          }
          else if(readString.indexOf("facacomarduino/Lampada2Off") > 0){
            if(readString.indexOf("facacomarduino/Lampada2Off") > 0)
            {
              digitalWrite(Pin2, LOW);  // Desliga Lampada 2.
            }
          }
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

