/**
 * @file ProjetoCofreWifi.ino
 * @author Leonardo Pereira Côrtes (200030582)
 * @brief Projeto Transversal de Redes de Comunicação 1 - Universidade de Brasília
 * @version 2 - módulo wifi
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2022
 */

#define BLYNK_TEMPLATE_ID "TMPLJnyTTPsN"
#define BLYNK_DEVICE_NAME "ProjetoCofre"
#define BLYNK_AUTH_TOKEN "q7w1oqI907tsBwEzBNhhjF2p1aBwgLLz"
#define BLYNK_PRINT Serial

// Bibliotecas
#include <Servo.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>

// Variaveis
Servo servo_motor;
int servo = 2;
int pos = 0;
String senha = "1234";

char auth[] = BLYNK_AUTH_TOKEN; // Declaracao da variavel que armazena o codigo de autenticacao para conexao
char ssid[] = "Casa Flavio"; // UNB wireless - Declaracao do nome e senha da rede Wi-Fi
char pass[] = "8008lpcl"; // MATU1214

const int ESP8266_BAUD = 9600; // Declaracao da variavel que armazena a velocidade de comunicacao do modulo
SoftwareSerial EspSerial(10, 11); // Criacao do objeto serial para comunicacao com o ESP8266 (RX, TX)
ESP8266 wifi(&EspSerial); // Confiracao do objeto 'wifi' para usar a serial do ESP8266 para conexao
WidgetTerminal terminal1(V0); // Terminal 1
WidgetTerminal terminal2(V2); // Terminal 2

BLYNK_WRITE(V0){ // Terminal 1
  if(String(senha) == param.asStr()){ // Se senha for correta
    terminal1.println("Senha correta!\nAbrindo cofre..");
    delay(500);
    for(pos=0; pos<=90; pos++) { // Mover o servo para 90°
      servo_motor.write(pos);              
      delay(15);
    }
    delay(1500);
    terminal1.println("Digite 'trancar' para trancar o cofre.");
  } else if(String("trancar") == param.asStr()){ // Se digitar 'trancar'
    terminal1.println("Trancando cofre..");
    for(pos=90; pos>=0; pos--) { // Mover o servo para 0°
      servo_motor.write(pos);              
      delay(15);                       
    }
  } else { // Senha errada
    terminal1.println("Senha incorreta.");
    terminal1.println("Tente novamente.");
  }
  terminal1.flush(); // Envia mensagens para o terminal
}

BLYNK_WRITE(V2){ // Terminal 2
  // Variaveis auxiliares
  String senhas;
  String senha_antiga;
  String senha_nova;

  senhas = param.asStr(); // senha_antiga + senha_nova
  senha_antiga = senhas.substring(0,4); // Separando as senhas
  senha_nova = senhas.substring(5,9);
  
  if(String(senha) == senha_antiga){ // Se a senha_antiga estiver correta
    terminal2.println("Senha correta.");
    senha = senhas.substring(5,9); // Torna a senha_nova como a senha
    terminal2.println("Senha alterada com sucesso.");
  } else { // Se a senha_antiga estiver errada
    terminal2.println("Senha incorreta.");
    terminal2.println("Tente novamente.");
  }
  terminal2.flush(); // Envia mensagens para o terminal
}

void setup(){

  Serial.begin(9600); //Inicializacao do monitor serial
  EspSerial.begin(ESP8266_BAUD); //Inicializa a comunicacao serial do ESP8266
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass); //Inicializacao da comunicacao e conexao do modulo ao aplicativo

  servo_motor.attach(servo);
  servo_motor.write(0); // Servo inicia fechado

  terminal1.clear(); // Limpa console do terminal
  terminal2.clear();
  delay(2000);
  terminal1.println(F("Digite a senha de 4 digitos para abrir o cofre."));
  terminal2.println(F("Digite a senha senha atual para trocar de senha."));
  terminal2.println(F("Formato: 'senha_antiga senha_nova'"));
  terminal1.flush();
  terminal2.flush();
}

void loop(){
  Blynk.run(); //Mantem a conexao ativa com o aplicativo e processa comandos recebidos ou enviados
}
