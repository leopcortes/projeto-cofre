/**
 * @file ProjetoCofre.ino
 * @author Leonardo Pereira Côrtes (200030582)
 * @brief Projeto Transversal de Redes de Comunicação 1 - Universidade de Brasília
 * @version 2
 * @date 2022-12-27
 * 
 * @copyright Copyright (c) 2022
 */

#include "Arduino.h"
#include <Servo.h> 
#include <LiquidCrystal.h>

// Pinos dos dispositivos
int buzzer = 2;
int servo = 3;
int botaoEnter = 4; 
int botao3 = 5;
int botao2 = 6;
int botao1 = 7;

const int RS = 13, EN = 12, D4 = 11, D5 = 10, D6 = 9, D7 = 8; // Pinos do LCD
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

Servo servo_motor;

char senha[3] = "123"; // Senha padrão
char textoDigitadoPeloUsuario[3]; // Variavel que armazena a senha digitada pelo usuário

// Variáveis auxiliares
bool abrirCofre = false, alterarSenha = false;
int pos=0; 
int tentativas=0; 
int a=0, p=0;

void escreveNoDisplay(int aux) { // Função para imprimir o número digitado no LCD
  if(aux == 1){
  lcd.print('1');
  textoDigitadoPeloUsuario[a]='1';
  }
  if(aux == 2){
  lcd.print('2');
  textoDigitadoPeloUsuario[a]='2';
  }
  if(aux == 3){
  lcd.print('3');
  textoDigitadoPeloUsuario[a]='3';
  }
  a++;
  delay(15);
}

void alteraSenha(int aux2) { // Função para alterar a senha
  if(aux2 == 1){
    lcd.print('1');
    senha[p]='1';
  }
  if(aux2 == 2){
    lcd.print('2');
    senha[p]='2';
  }
  if(aux2 == 3){
    lcd.print('3');
    senha[p]='3';
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
  delay(1000);  
  
  for (pos = 0; pos <= 90; pos += 1) { // Mover o servo em 90°
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
  abrirCofre = false; 
  alterarSenha = false;
  a = 0;
  p = 0;  
}

void alarme() { // Função para emitir o alarme no buzzer
  lcd.clear(); 
  lcd.setCursor(4,0);
  lcd.print("ALARME!!");
  for (int vezesQueAlarmeToca=0; vezesQueAlarmeToca<10; vezesQueAlarmeToca++) { // Toca 10 vezes
    for (int freq = 500; freq<1000; freq++) {
      tone(buzzer, freq);
      delayMicroseconds(600);
    }
  }
  noTone(buzzer);
}

void setup() { // Setup inicial
  lcd.begin (16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1- Abrir o cofre");
  lcd.setCursor(0,1);
  lcd.print("2- Alterar senha");
  
  // Define os botões como entrada
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(botaoEnter, INPUT);
  
  // Inicializa o servo
  servo_motor.attach(servo);
  servo_motor.write(0); 
}
 
void loop() {
  if (abrirCofre == false && alterarSenha == false) { // Tela inicial, espera a escolha do usuário
    if(digitalRead(botao1) == HIGH){ // Se apertar 1, entra na operação de abrir o cofre
      abrirCofre = true;
      while(digitalRead(botao1) == HIGH){}
      resetaCofre(); 
    }
    if(digitalRead(botao2) == HIGH) { // Se apertar 2, entra na operação de trocar de senha
      alterarSenha = true;
      while(digitalRead(botao2) == HIGH){}
      resetaCofre();
    }
  }

  if (alterarSenha == true) { // Operação para trocar de senha
    if (a == 3){ 
      if(digitalRead(botaoEnter) == HIGH) { 
        lcd.clear(); 
        
        for(int x=0; x<3; x++){
          if(senha[x] != textoDigitadoPeloUsuario[x]){
            x = 3;
            if(tentativas == 2) {
              alarme();
              telaInicial(); 
              tentativas=0;
            }
            else {
              senhaIncorreta();  
            }
          }
          if (x == 2) {
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
    }
    else if (a < 3) {
      int aux = 0;
      if(digitalRead(botao1) == HIGH){
        aux = 1;
        escreveNoDisplay(aux);
        while(digitalRead(botao1)==HIGH){}
      }
      if(digitalRead(botao2) == HIGH){
        aux = 2;
        escreveNoDisplay(aux);
        while(digitalRead(botao2)==HIGH){}
      }
      if(digitalRead(botao3) == HIGH){
        aux = 3;
        escreveNoDisplay(aux);
        while(digitalRead(botao3)==HIGH){}
      }
    }
    else if (a > 3) {
      if(p < 3){
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
      }
      if (p == 3){ 
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
  
  if (abrirCofre == true) { // Operação para abrir o cofre
    if (a == 3){ 
      if(digitalRead(botaoEnter) == HIGH) { 
        lcd.clear(); 
        
        for (int x=0; x<3; x++){
          if (senha[x]!=textoDigitadoPeloUsuario[x]){
              x = 3;
            if(tentativas == 2) {
              alarme();
              telaInicial(); 
              tentativas = 0;
            }
            else {
              senhaIncorreta();  
            }
          }
          if(x == 2) {
            senhaCorreta();
            tentativas = 0;
          }
        }
      }
    }
    else if (a < 3) {
      int aux3 = 0;
      if(digitalRead(botao1) == HIGH){
        aux3 = 1;
        escreveNoDisplay(aux3);
        while(digitalRead(botao1)==HIGH){}
      }
      if(digitalRead(botao2) == HIGH){
        aux3 = 2;
        escreveNoDisplay(aux3);
        while(digitalRead(botao2)==HIGH){}
      }
      if(digitalRead(botao3) == HIGH){
        aux3 = 3;
        escreveNoDisplay(aux3);
        while(digitalRead(botao3)==HIGH){}
      }
    }
    else if (a > 3) {
      if(digitalRead(botaoEnter) == HIGH){  
        lcd.clear(); 
        lcd.setCursor(0,0);
        lcd.print("Trancando o");
        lcd.setCursor(0,1);
        lcd.print("Cofre...");
        delay(1000);
        for(pos = 90; pos >= 0; pos --) { 
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
}
