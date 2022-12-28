#include <LiquidCrystal.h> // inclui a biblioteca para uso do Display LCD
// inicializa um objeto nos pinos para acesso as funções do LCD
const int RS = 7, EN = 6, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup(){
  // Configura o LCD com os número de colunas e linhas
  lcd.begin(16, 2); // 16 colunas e 2 linhas

  // Limpa o LCD
  lcd.clear();
  
  // posiciona o cursor do LCD
  lcd.setCursor(0, 0); // (coluna 0, linha 0)
  lcd.print("Kit Avancado RC"); // Imprime mensagem
}

void loop(){
  // Calcula o tempo decorrido em segundos desde que o Arduino foi iniciado
  unsigned long tempo_decorrido = millis() / 1000; // ms -> s

  // Imprime no LCD
  lcd.setCursor(0, 1); // (coluna 0, linha 1)
  lcd.print(tempo_decorrido);
  lcd.print("s");
}
