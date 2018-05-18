  /* 
  Projeto original de Pedro Minatel.
  Retirado do site: http://pedrominatel.com.br/pt/arduino/web-server-com-esp8266-e-ide-arduino/
  */

  #include <ESP8266WiFi.h>
  
  IPAddress ip(192,168,0,111);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  
  WiFiServer server(8085);

  int status ;
  int rele = 12;    // sonof = 12
  int led = 13;     // sonof = 13
  int botao = 0;   // sonof = 0
  int estado_ch1 = 0;
  
  void setup() {

  WiFi.begin("Vitorelo", "setembro1309");
  WiFi.config(ip, gateway, subnet);  

  Serial.begin(9600);
  delay(10);
  
  pinMode(rele, OUTPUT);
  digitalWrite(rele, 0);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  pinMode(botao, INPUT_PULLUP);
    
  conectar();
  }

  void conectar() {
  Serial.println();
  Serial.println();
  Serial.print("Connectando a ");
  while (WiFi.status() != WL_CONNECTED) {
  delay(2000);
  Serial.print(".");
  Serial.println("");
  Serial.println("Conectado com sucesso na rede.");
  // Start the server
  server.begin();
  Serial.println("IP Obtido: ");
  Serial.print(WiFi.localIP());
  }}

  void Ligadesliga() {
  Canal_01:
  if(digitalRead(botao)== 0 && estado_ch1 == 0)
  { digitalWrite(rele,!digitalRead(rele));
    digitalWrite(led, digitalRead(rele));
    delay(400);
    estado_ch1 = 1;}
  if(digitalRead(botao) == 1 && estado_ch1 == 1)
  { delay(400);
    estado_ch1 = 0;}
  }

  void loop() {
  Ligadesliga();
     
  if (WiFi.status() != WL_CONNECTED) {conectar();}
  WiFiClient client = server.available();
  if (!client) { 
    return; 
  }     
  
  String req = client.readStringUntil('\r');
  client.flush();

  if (req.indexOf("D0_on") != -1){
    digitalWrite(rele, 1);
    digitalWrite(led, 1);}
    
  if (req.indexOf("D0_off") != -1){
    digitalWrite(rele, 0);
    digitalWrite(led, 0);}
    
  if (digitalRead(rele) == 1)
  { status = 1; }
  if (digitalRead(rele) == 0)
  { status = 0; }
  
  String buf = "";
 
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<html lang=\"pt\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>\r\n";
  buf += "<meta http-equiv='refresh' content='2;url=/'>";
  buf += "<link rel='icon' type='image/png' href='https://icon-icons.com/icons2/474/PNG/512/light-bulb_46866.png'/>"; 
  buf += "<body style=background-color:#000000>";
  buf += "<title>CONTROLE ILUMINA&Ccedil;&Atilde;O</title>";
  buf += "<p style= font-size:30px><b><font color=#0000FF>CONTROLE ILUMINA&Ccedil;&Atilde;O</font></p></b>";
  buf += "<hr align='center' width='100%' size='3' color='#FFFFFF'>"; // Cria uma Linha Horizontal.
  buf += "<p style= font-size:16px><b><font color=#FFFFFF>CANAL 01</font></p></b>";
  buf += "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:80%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#FFA500;color:#FFFFF;line-height:2.0rem;font-size:1.2rem;width:49%;} .q{float: right;width: 64px;text-align: right;}</style>";
  buf += "<a href=\"?function=D0_on\"><button><b>LIGA</b></button></a> <a href=\"?function=D0_off\"><button><b>DESLIGA</b></button></a></p>";
  
  if (status == 1){
    buf += "<p style= font-size:20px><b><font color=#008000>SONOFF LIGADO</font></p></b>";} //ESCREVE "LÂMPADA LIGADA" NA PÁGINA
  if (status == 0){
    buf += "<p style= font-size:20px><b><font color=#FF0000>SONOFF DESLIGADO</font></p></b>";} //ESCREVE "LÂMPADA DESLIGADA" NA PÁGINA
    
    buf += "<hr align='center' width='100%' size='3' color='#FFFFFF'>"; // Cria uma Linha Horizontal.
   
  client.print(buf);
  delay(10);
  client.stop();
  }

