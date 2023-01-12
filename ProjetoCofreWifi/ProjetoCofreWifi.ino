#define BLYNK_TEMPLATE_ID "TMPLJnyTTPsN"
#define BLYNK_DEVICE_NAME "ESP8266"
#define BLYNK_AUTH_TOKEN "q7w1oqI907tsBwEzBNhhjF2p1aBwgLLz"

//Definicao do monitoramento de conexao da placa pela serial
#define BLYNK_PRINT Serial

//Adicao das bibliotecas
#include <Servo.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>

Servo servo_motor;

int servo = 2;
int pos = 0; 

//Declaracao da variavel que armazena o codigo de autenticacao para conexao
char auth[] = BLYNK_AUTH_TOKEN;

//Declaracao do nome e senha da rede Wi-Fi
//Altere as variaveis abaixo com o nome e senha da sua rede Wi-Fi
char ssid[] = "Casa Flavio"; // UNB wireless - Declaracao do nome e senha da rede Wi-Fi
char pass[] = "8008lpcl";

//Criacao do objeto serial para comunicacao com o ESP8266
SoftwareSerial EspSerial(10, 11); // RX, TX

//Declaracao da variavel que armazena a velocidade de comunicacao do modulo
const int ESP8266_BAUD = 9600;

//Confiracao do objeto 'wifi' para usar a serial do ESP8266 para conexao
ESP8266 wifi(&EspSerial);

WidgetTerminal terminal(V0);

BLYNK_WRITE(V0){
  if(String("1234") == param.asStr()){
    terminal.println("Senha correta!");
    for(pos=0; pos<=90; pos++) { // Mover o servo em 90°
      servo_motor.write(pos);              
      delay(15);                       
    }
  } else if(String("trancar") == param.asStr()){
    for(pos = 90; pos>=0; pos--) { 
      servo_motor.write(pos);              
      delay(15);                       
    } 
  } else {
    terminal.println("Senha incorreta.");
    terminal.println("Tente novamente.");
  }
  terminal.flush();
}


BLYNK_WRITE(V1){ //Funcao que le o pino V1 a cada atualizacao de estado
  int pinValue = param.asInt(); //Le o valor do pino virtual
  digitalWrite(LED_BUILTIN, pinValue); //Aciona o LED da placa de acordo com o valor lido pelo pino virtual
}

//Configuracao do codigo
void setup(){

  //Inicializacao do monitor serial
  Serial.begin(9600);

  //Configura o pino do LED interno da placa como saida
  pinMode(LED_BUILTIN, OUTPUT);

  //Inicializa a comunicacao serial do ESP8266
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  //Inicializacao da comunicacao e conexao do modulo ao aplicativo
  Blynk.begin(auth, wifi, ssid, pass);

  servo_motor.attach(servo);
  servo_motor.write(0);

  terminal.println(F("AAAAAAAAA"));
  terminal.flush();
}

//Repeticao do codigo
void loop(){
  Blynk.run(); //Mantem a conexao ativa com o aplicativo e processa comandos recebidos ou enviados
}
