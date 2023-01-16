/**
 * @file ProjetoCofre.ino
 * @author Leonardo Pereira Côrtes (200030582)
 * @brief Projeto Transversal de Redes de Comunicação 1 - Universidade de Brasília
 * @version 3
 * @date 2022-12-29
 * 
 * @copyright Copyright (c) 2022
 *//*
#define BLYNK_TEMPLATE_ID "TMPLJnyTTPsN"
#define BLYNK_DEVICE_NAME "ProjetoCofre"
#define BLYNK_AUTH_TOKEN "q7w1oqI907tsBwEzBNhhjF2p1aBwgLLz"
#define BLYNK_PRINT Serial // Definicao do monitoramento de conexao da placa pela serial
*/
// Bibliotecas
#include "Arduino.h"
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
/*#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>*/

// Pinos dos dispositivos
const int servo = 2;
const int botaoEnter = 3;
const int botao4 = 4;
const int botao3 = 5;
const int botao2 = 6;
const int botao1 = 7;
const int buzzer = 13;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo_motor;

// Variáveis auxiliares
//bool abrirCofre = false, alterarSenha = false;
//bool abrirCofreWifi = false, trocarSenhaWifi = false, controlarCelular = false;
int pos = 0; 
int tentativas = 0; 
int a = 0, p = 0;

char senha[4] = {'1','2','3','4'}; // Senha padrão
char textoDigitadoPeloUsuario[4]; // Variavel que armazena a senha digitada pelo usuário
/*
// Declarações do módulo wifi
char auth[] = BLYNK_AUTH_TOKEN; // Codigo de autenticacao para conexao
char ssid[] = "Casa Flavio"; // UNB Wireless - Declaracao do nome e senha da rede Wi-Fi
char pass[] = "8008lpcl"; // MATU1214
SoftwareSerial EspSerial(10, 11); // Criacao do objeto serial para comunicacao com o ESP8266 - RX, TX
const int ESP8266_BAUD = 9600; // Velocidade de comunicacao do modulo
ESP8266 wifi(&EspSerial); // Confiracao do objeto 'wifi' para usar a serial do ESP8266 para conexao
WidgetTerminal terminal(V0);

BLYNK_WRITE(V0){
  if(String("1234") == param.asStr()){
    terminal.println("Senha correta!");
    for(pos=0; pos<=90; pos++) { // Mover o servo em 90°
      servo_motor.write(pos);              
      delay(15);                       
    }
  } else if(String("trancar") == param.asStr()){
    terminal.println("Trancado cofre..");
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
*/
void escreveNoDisplay(int aux) { // Função para imprimir o número digitado no LCD
  if(aux == 1){
  lcd.print('1');
  textoDigitadoPeloUsuario[a] = '1';
  }
  if(aux == 2){
  lcd.print('2');
  textoDigitadoPeloUsuario[a] = '2';
  }
  if(aux == 3){
  lcd.print('3');
  textoDigitadoPeloUsuario[a] = '3';
  }
  if(aux == 4){
  lcd.print('4');
  textoDigitadoPeloUsuario[a] = '4';
  }
  a++;
  delay(15);
}

void alteraSenha(int aux2) { // Função para alterar a senha
  if(aux2 == 1){
    lcd.print('1');
    senha[p] = '1';
  }
  if(aux2 == 2){
    lcd.print('2');
    senha[p] = '2';
  }
  if(aux2 == 3){
    lcd.print('3');
    senha[p] = '3';
  }
  if(aux2 == 4){
    lcd.print('4');
    senha[p] = '4';
  }
  p++;
  delay(15);
}

void resetaCofre() { // Função para resetar o LCD
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Digite a senha:");
  lcd.setCursor(6,1);
  a = 0;
}

void senhaIncorreta() { // Função para quando usuário errar a senha
  lcd.setCursor(0,0);
  lcd.print("Senha Incorreta");
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("Tente novamente");
  delay(2000);
  resetaCofre();
  tentativas++;
}

void senhaCorreta() { // Função para quando usuário acertar a senha
  lcd.setCursor(0,0);
  lcd.print("Senha Correta");
  //Blynk.logEvent("O cofre foi aberto.");
  delay(1000);  
  for(pos=0; pos<=90; pos++) { // Mover o servo em 90°
    servo_motor.write(pos);              
    delay(15);                       
  }
  lcd.setCursor(0,1);
  lcd.print("Cofre Aberto");
  a++;
  delay(5000);
}

void telaInicial() { // Função para imprimir a tela inicial
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("1- Abrir o cofre");
  lcd.setCursor(0,1);
  lcd.print("2- Alterar senha");
  //abrirCofre = false; 
  //alterarSenha = false;
  a = 0;
  p = 0;  
}

void alarme() { // Função para emitir o alarme no buzzer
  lcd.clear(); 
  lcd.setCursor(4,0);
  lcd.print("ALARME!!");
  //Blynk.logEvent("ALARME COFRE!!");
  for (int vezesQueAlarmeToca=0; vezesQueAlarmeToca<10; vezesQueAlarmeToca++) { // Toca 10 vezes
    for(int freq = 500; freq<1000; freq++) {
      tone(buzzer, freq);
      delayMicroseconds(600);
    }
  }
  noTone(buzzer);
}
 
void alterarSenha() {
  if(a == 4){ 
    if(digitalRead(botaoEnter) == HIGH) { 
      lcd.clear(); 
      
      if(senha != textoDigitadoPeloUsuario){
        if(tentativas == 2) {
          alarme();
          telaInicial(); 
          tentativas = 0;
        }
        else {
          senhaIncorreta();  
        }
      }
      if(senha == textoDigitadoPeloUsuario) {
        a++;
        tentativas = 0;
        lcd.setCursor(0,0);
        lcd.print("Senha Correta");
        delay(2500);
        lcd.clear(); 
        lcd.setCursor(0,0);
        lcd.print("Digite a nova");
        lcd.setCursor(0,1);
        lcd.print("senha ...");
        delay(2500);
        lcd.clear(); 
        lcd.setCursor(0,0);
        lcd.print("Nova Senha:");
        lcd.setCursor(6,1);             
      }
    }
  }
  else if(a < 4) {
    int aux = 0;
    if(digitalRead(botao1) == HIGH){
      aux = 1;
      escreveNoDisplay(aux);
      while(digitalRead(botao1) == HIGH){}
    }
    if(digitalRead(botao2) == HIGH){
      aux = 2;
      escreveNoDisplay(aux);
      while(digitalRead(botao2) == HIGH){}
    }
    if(digitalRead(botao3) == HIGH){
      aux = 3;
      escreveNoDisplay(aux);
      while(digitalRead(botao3) == HIGH){}
    }
    if(digitalRead(botao4) == HIGH){
      aux = 4;
      escreveNoDisplay(aux);
      while(digitalRead(botao4) == HIGH){}
    }
  }
  else if(a > 4) {
    if(p < 4){
      int aux2 = 0;
      if(digitalRead(botao1) == HIGH){
        aux2 = 1;
        alteraSenha(aux2);
        while(digitalRead(botao1) == HIGH){}
      }
      if(digitalRead(botao2) == HIGH){
        aux2 = 2;
        alteraSenha(aux2);
        while(digitalRead(botao2) == HIGH){}
      }
      if(digitalRead(botao3) == HIGH){
        aux2 = 3;
        alteraSenha(aux2);
        while(digitalRead(botao3) == HIGH){}
      }
      if(digitalRead(botao4) == HIGH){
        aux2 = 4;
        alteraSenha(aux2);
        while(digitalRead(botao4) == HIGH){}
      }
    }
    if(p == 4){ 
      if (digitalRead(botaoEnter) == HIGH) { 
        lcd.clear(); 
        lcd.setCursor(0,0);
        lcd.print("Senha alterada");
        lcd.setCursor(0,1);
        lcd.print("com sucesso...");
        delay(3000);
        telaInicial();
      }
    }
  }  
}

void abrirCofre() {
  if(a == 4) { // Se usuário digitar os 4 caracters da senha
    if(digitalRead(botaoEnter) == HIGH) { // Se apertar enter
      lcd.clear(); 
      
      if(senha != textoDigitadoPeloUsuario){ // Compara o n° digitado pelo usuário com o caractere da senha
        if(tentativas == 2) { // Se for o 3° erro ativa o alarme
          alarme();
          telaInicial();
          tentativas = 0;
        }
        else { // Se for 1° ou 2° erro conta +1 tentativa
          senhaIncorreta();
        }
      }
      if(senha == textoDigitadoPeloUsuario) { // Se a senha for correta
        senhaCorreta();
        tentativas = 0;
      }
    }
  }
  else if(a < 4) { // Se não tiver digitado os 4 digitos ainda faz a leitura do próximo digito
    int aux3 = 0;
    if(digitalRead(botao1) == HIGH){ // 1
      aux3 = 1;
      escreveNoDisplay(aux3);
      while(digitalRead(botao1) == HIGH){}
    }
    if(digitalRead(botao2) == HIGH){ // 2
      aux3 = 2;
      escreveNoDisplay(aux3);
      while(digitalRead(botao2) == HIGH){}
    }
    if(digitalRead(botao3) == HIGH){ // 3
      aux3 = 3;
      escreveNoDisplay(aux3);
      while(digitalRead(botao3) == HIGH){}
    }
    if(digitalRead(botao4) == HIGH){ // 4
      aux3 = 4;
      escreveNoDisplay(aux3);
      while(digitalRead(botao4) == HIGH){}
    }
  }
  else if(a > 4) { // Depois da senha, se apertar o botão enter tranca o cofre
    if(digitalRead(botaoEnter) == HIGH){  
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("Trancando o");
      lcd.setCursor(0,1);
      lcd.print("Cofre...");
      delay(1000);
      for(pos = 90; pos>=0; pos--) { // Volta o servo para 0º
        servo_motor.write(pos);              
        delay(15);                       
      } 
      delay(1500);
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("Cofre Trancado");
      delay(2500);
      telaInicial(); 
    }
  }
}

void setup() { // Setup inicial
  /*Serial.begin(9600); // Inicializacao do monitor serial
  delay(10);
  EspSerial.begin(ESP8266_BAUD); //Inicializa a comunicacao serial do ESP8266
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass); // Inicializacao da comunicacao e conexao do modulo ao aplicativo
*/
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1- Abrir o cofre");
  lcd.setCursor(0,1);
  lcd.print("2- Alterar senha");
  
  // Define os botões como entrada
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(botao4, INPUT);
  pinMode(botaoEnter, INPUT);
  //pinMode(LED_BUILTIN, OUTPUT); // LED interno como saída
  
  // Inicializa o servo
  servo_motor.attach(servo);
  servo_motor.write(0);
/*
  delay(500);
  
  terminal.println(F("Digite a senha para abrir o cofre."));
  terminal.flush();*/
}

void loop() {
  //Blynk.run(); //Mantem a conexao ativa com o aplicativo e processa comandos recebidos ou enviados
  
  if(digitalRead(botao1) == HIGH){ // Se apertar 1, entra na operação de abrir o cofre
    alterarSenha();
    while(digitalRead(botao1) == HIGH){}
    resetaCofre(); 
  }
  if(digitalRead(botao2) == HIGH) { // Se apertar 2, entra na operação de trocar de senha
    abrirCofre();
    while(digitalRead(botao2) == HIGH){}
    resetaCofre();
  }
}
