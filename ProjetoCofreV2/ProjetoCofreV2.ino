/**
 * @file ProjetoCofreV2.ino
 * @author Leonardo Pereira Côrtes (200030582)
 * @brief Projeto Transversal de Redes de Comunicação 1 - Universidade de Brasília
 * @version 2 - sem módulo wifi
 * @date 2023-01-12
 * 
 * @copyright Copyright (c) 2022
 */

// Bibliotecas
#include "Arduino.h"
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pinos dos dispositivos
int servo = 2;
int botaoEnter = 3;
int botao4 = 4;
int botao3 = 5;
int botao2 = 6;
int botao1 = 7;
int buzzer = 13;

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD com 2 linhas e 16 colunas
Servo servo_motor;

char senha[4] = {'1','2','3','4'}; // Senha padrão
char textoDigitadoPeloUsuario[4]; // Variavel que armazena a senha digitada pelo usuário

// Variáveis auxiliares
bool abrirCofre = false, alterarSenha = false;
int pos = 0; 
int tentativas = 0; 
int a = 0, p = 0;

void escreveNoDisplay(int aux) { // Função para imprimir o número digitado no LCD
  if(aux == 1){ // Se digitar x imprime o numero no LCD e insere no array que guarda o texto digitado pelo usuario
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
  a++; // Soma 1 na contagem de numeros da senha ja digitados
  delay(15);
}

void alteraSenha(int aux2) { // Função para alterar a senha
  if(aux2 == 1){ // Se digitar x imprime o numero no LCD e insere no array que guarda o texto digitado pelo usuario
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
  p++; // Soma 1 na contagem de numeros da troca de senha ja digitados
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
  tentativas++; // Soma 1 nas tentativas
}

void senhaCorreta() { // Função para quando usuário acertar a senha
  lcd.setCursor(0,0);
  lcd.print("Senha Correta");
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
  abrirCofre = false; 
  alterarSenha = false;
  a = 0;
  p = 0;  
}

void alarme() { // Função para emitir o alarme no buzzer
  lcd.clear(); 
  lcd.setCursor(4,0);
  lcd.print("ALARME!!");
  for (int vezesQueAlarmeToca=0; vezesQueAlarmeToca<10; vezesQueAlarmeToca++) { // Toca o alarme 10 vezes
    for(int freq = 500; freq<1000; freq++) {
      tone(buzzer, freq);
      delayMicroseconds(600);
    }
  }
  noTone(buzzer); // Para o alarme
}

void setup() { // Setup inicial
  // Texto inicial do LCD
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
  
  // Inicializa o servo fechado
  servo_motor.attach(servo);
  servo_motor.write(0);
}
 
void loop() {
  if(abrirCofre == false && alterarSenha == false) { // Tela inicial, espera a escolha do usuário
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

  if(alterarSenha == true) { // Operação para trocar de senha
    if(a == 4){ // Se usuário digitar os 4 caracters da senha
      if(digitalRead(botaoEnter) == HIGH) { // Se apertar enter
        lcd.clear(); 
        
        for(int x=0; x<4; x++){ // Para cada digito
          if(senha[x] != textoDigitadoPeloUsuario[x]){ // Compara o n° digitado pelo usuário com o caractere da senha
            x = 4;
            if(tentativas == 2) { // Se for o 3° erro ativa o alarme
              alarme();
              telaInicial(); 
              tentativas = 0;
            }
            else { // Se for 1° ou 2° erro conta +1 tentativa
              senhaIncorreta();  
            }
          }
          if(x == 3) { // Se a senha for correta esta pronto para mudar de senha
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
    else if(a < 4) { // Se não tiver digitado os 4 digitos ainda faz a leitura do próximo digito
      int aux = 0;
      if(digitalRead(botao1) == HIGH){ // 1
        aux = 1;
        escreveNoDisplay(aux);
        while(digitalRead(botao1) == HIGH){}
      }
      if(digitalRead(botao2) == HIGH){ // 2
        aux = 2;
        escreveNoDisplay(aux);
        while(digitalRead(botao2) == HIGH){}
      }
      if(digitalRead(botao3) == HIGH){ // 3
        aux = 3;
        escreveNoDisplay(aux);
        while(digitalRead(botao3) == HIGH){}
      }
      if(digitalRead(botao4) == HIGH){ // 4
        aux = 4;
        escreveNoDisplay(aux);
        while(digitalRead(botao4) == HIGH){}
      }
    }
    else if(a > 4) { // Depois da senha, espera a nova senha
      if(p < 4){ // Equanto nao tiver digitado os 4 numeros, faz a leitura
        int aux2 = 0;
        if(digitalRead(botao1) == HIGH){ // 1
          aux2 = 1;
          alteraSenha(aux2);
          while(digitalRead(botao1) == HIGH){}
        }
        if(digitalRead(botao2) == HIGH){ // 2
          aux2 = 2;
          alteraSenha(aux2);
          while(digitalRead(botao2) == HIGH){}
        }
        if(digitalRead(botao3) == HIGH){ // 3
          aux2 = 3;
          alteraSenha(aux2);
          while(digitalRead(botao3) == HIGH){}
        }
        if(digitalRead(botao4) == HIGH){ // 4
          aux2 = 4;
          alteraSenha(aux2);
          while(digitalRead(botao4) == HIGH){}
        }
      }
      if(p == 4){ // Se usuário digitar os 4 caracteres
        if(digitalRead(botaoEnter) == HIGH) { // Se apertar enter confirma a troca de senha
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
  
  if(abrirCofre == true) { // Operação para abrir o cofre
    if(a == 4) { // Se usuário digitar os 4 caracters da senha
      if(digitalRead(botaoEnter) == HIGH) { // Se apertar enter
        lcd.clear(); 
        
        for(int x=0; x<4; x++){ // Para cada digito
          if(senha[x] != textoDigitadoPeloUsuario[x]){ // Compara o n° digitado pelo usuário com o caractere da senha
              x = 4;
            if(tentativas == 2) { // Se for o 3° erro ativa o alarme
              alarme();
              telaInicial();
              tentativas = 0;
            }
            else { // Se for 1° ou 2° erro conta +1 tentativa
              senhaIncorreta();
            }
          }
          if(x == 3) { // Se a senha for correta abre o cofre
            senhaCorreta();
            tentativas = 0;
          }
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
        for(pos = 90; pos>=0; pos--) { 
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
