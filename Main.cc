#include <LiquidCrystal.h>

int buzzer = 6;
int bot1 = 10;
int bot2 = 9;
int bot_reset = 13;

// LEDs
int led1 = 7;
int led2 = 8;

int fase = 1; // vai controlar as fases do jogo
bool jogandoMemoria = true;

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

// Tamanho da sequência da memória
#define SEQUENCIA_TAMANHO 4
int sequencia[SEQUENCIA_TAMANHO];
int tentativa[SEQUENCIA_TAMANHO];
int posicaoAtual = 0; // Para verificar a sequência que o jogador está tentando

// Função para gerar uma sequência aleatória de LEDs
void gerarSequencia() {
  for (int i = 0; i < SEQUENCIA_TAMANHO; i++) {
    sequencia[i] = random(0, 2); // 0 para led1 e 1 para led2
  }
}

// Mostrar a sequência de LEDs
void mostrarSequencia() {
  for (int i = 0; i < SEQUENCIA_TAMANHO; i++) {
    if (sequencia[i] == 0) {
      digitalWrite(led1, HIGH);
      delay(500);
      digitalWrite(led1, LOW);
    } else {
      digitalWrite(led2, HIGH);
      delay(500);
      digitalWrite(led2, LOW);
    }
    delay(300); 
  }
}

//Reproduz musica de acerto
void somAcerto() {
  tone(buzzer, 196, 250); 
  delay(325);                    
  tone(buzzer, 196, 125); 
  delay(160);                    
  tone(buzzer, 220, 125); 
  delay(160);                    
  tone(buzzer, 220, 325); 
  delay(325);                    
  
  noTone(buzzer); 
}

//Reproduz musica de erro
void somErro() {
  tone(buzzer, 196, 125); 
  delay(230);                    
  tone(buzzer, 196, 125); 
  delay(230);                    
  tone(buzzer, 196, 125); 
  delay(230);                    
  tone(buzzer, 196, 125); 
  delay(230);                    
  
  noTone(buzzer); 
}

// Função para verificar a sequência
bool verificarTentativa(int tentativa[], int tamanho) {
  for (int i = 0; i < tamanho; i++) {
    if (tentativa[i] != sequencia[i]) {
      return false; 
    }
  }
  return true; 
}

void setup() {
  lcd_1.begin(16, 2);
  pinMode(bot1, INPUT); // Botão 1
  pinMode(bot2, INPUT); // Botão 2
  pinMode(bot_reset, INPUT); // Botão RESET
  pinMode(buzzer, OUTPUT); // Buzzer
  pinMode(led1, OUTPUT); // LED 1
  pinMode(led2, OUTPUT); // LED 2
  
  // Desligar o buzzer
  noTone(buzzer);
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Seja bem-vindo!");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Pressione reset");

  // Esperar o jogador iniciar o jogo
  while (digitalRead(bot_reset) == LOW) {
    // Aguarda até pressionar o botão reset
  }

  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Iniciando Jogo...");
  delay(2000);

  // Gerar a sequência inicial
  gerarSequencia();
}

void loop() {
  int button_True = digitalRead(bot1);
  int button_False = digitalRead(bot2);
  int button_Reset = digitalRead(bot_reset);
	
  // Se o botão reset for pressionado, reinicia o jogo
  if (button_Reset == HIGH) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Reiniciando...");
    delay(2000);
    fase = 1;
    gerarSequencia();
    posicaoAtual = 0;
    jogandoMemoria = true;
    mostrarSequencia();
  }

  // Fase 1: Jogo da memória
  if (fase == 1 && jogandoMemoria) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Fase da Memoria");
    delay(2000);
    
    mostrarSequencia();
    
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Repita a seq");

    while (posicaoAtual < SEQUENCIA_TAMANHO) {
      // Atualiza a leitura dos botões dentro do loop
      button_True = digitalRead(bot1);
      button_False = digitalRead(bot2);

      if (button_True == HIGH) {
        tentativa[posicaoAtual] = 0; // Tentativa de LED 1
        digitalWrite(led1, HIGH);
        delay(500);
        digitalWrite(led1, LOW);
        
        // Mostra no LCD a sequência e acertos
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Sequencia: ");
        lcd_1.setCursor(10, 0);
        lcd_1.print(posicaoAtual + 1);
        lcd_1.setCursor(0, 1);
        lcd_1.print("Acertou!");
        delay(1000);
        
        posicaoAtual++; // Avança para a próxima posição

      } else if (button_False == HIGH) {
        tentativa[posicaoAtual] = 1; // Tentativa de LED 2
        digitalWrite(led2, HIGH);
        delay(500);
        digitalWrite(led2, LOW);
        
        // Mostra no LCD a sequência e erro
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Sequencia: ");
        lcd_1.setCursor(10, 0);
        lcd_1.print(posicaoAtual + 1);
        lcd_1.setCursor(0, 1);
        lcd_1.print("Acertou!");
        delay(1000);
        
        posicaoAtual++; // Avança para a próxima posição
      }

      delay(100); // Pequeno delay para evitar leituras rápidas
    }

    // Após completar a sequência
    if (verificarTentativa(tentativa, posicaoAtual)) {
      somAcerto();
      lcd_1.clear();
      lcd_1.setCursor(0, 1);
      lcd_1.print("Acertou!");
      delay(2000);
      fase = 2; // Avança para a próxima fase
      jogandoMemoria = false;
    } else {
      somErro();
      lcd_1.clear();
      lcd_1.setCursor(0, 0);
      lcd_1.print("Errou! Reiniciando...");
      delay(2000);
      
      // Reinicia o jogo
      fase = 1;
      gerarSequencia();
      posicaoAtual = 0;
      lcd_1.clear();
      lcd_1.setCursor(0, 0);
      lcd_1.print("Seja bem-vindo!");
      lcd_1.setCursor(0, 1);
      lcd_1.print("Pressione reset");
    }
  }

  // Fase 2 - Pergunta 1
  if (fase == 2) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Pergunta 1:");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Terra e redonda?");

    while (true) {
      // Atualiza a leitura dos botões dentro do loop
      button_True = digitalRead(bot1);
      button_False = digitalRead(bot2);

      if (button_True == HIGH) {
        // Resposta correta
        somAcerto();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Acertou!");
        delay(2000);
        fase = 3; // Avançar para a fase final
        break;
      } else if (button_False == HIGH) {
        // Resposta errada
        somErro();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Errou!");
        delay(2000);
        fase = 1; // Volta para a primeira fase
        break;
      }
    }
  }
  
  // Fase 3 - Pergunta 2
  if (fase == 3) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Pergunta 2:");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Marte eh azul?");

    while (true) {
      // Atualiza a leitura dos botões dentro do loop
      button_True = digitalRead(bot1);
      button_False = digitalRead(bot2);

      if (button_False == HIGH) {
        // Resposta correta
        somAcerto();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Acertou!");
        delay(2000);
        fase = 4; // Avançar para a proxima fase 
        break;
      } else if (button_True == HIGH) {
        // Resposta errada
        somErro();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Errou!");
        delay(2000);
        fase = 1; // Volta para a primeira fase
        break;
      }
    }
  }

  // Fase 4 - Pergunta 3
  if (fase == 4) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Pergunta 3:");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Agua e colorida?");

    while (true) {
      // Atualiza a leitura dos botões dentro do loop
      button_True = digitalRead(bot1);
      button_False = digitalRead(bot2);

      if (button_False == HIGH) {
        // Resposta correta
        somAcerto();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Acertou!");
        delay(2000);
        fase = 5; // Avançar para a proxima fase 
        break;
      } else if (button_True == HIGH) {
        // Resposta errada
        somErro();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Errou!");
        delay(2000);
        fase = 1; // Volta para a primeira fase
        break;
      }
    }
  }
  
  // Fase 5 - Pergunta 4
  if (fase == 5) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Pergunta 4:");
    lcd_1.setCursor(0, 1);
    lcd_1.print("CPU == PC ?");

    while (true) {
      // Atualiza a leitura dos botões dentro do loop
      button_True = digitalRead(bot1);
      button_False = digitalRead(bot2);

      if (button_False == HIGH) {
        // Resposta correta
        somAcerto();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Acertou!");
        delay(2000);
        fase = 6; // Avançar para a proxima fase 
        break;
      } else if (button_True == HIGH) {
        // Resposta errada
        somErro();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Errou!");
        delay(2000);
        fase = 1; // Volta para a primeira fase
        break;
      }
    }
  }
  
  // Fase 6 - Pergunta 5
  if (fase == 6) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Pergunta 5:");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Android > Apple?");

    while (true) {
      // Atualiza a leitura dos botões dentro do loop
      button_True = digitalRead(bot1);
      button_False = digitalRead(bot2);

      if (button_True == HIGH) {
        // Resposta correta
        somAcerto();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Acertou!");
        delay(2000);
        fase = 7; // Avançar para a proxima fase 
        break;
      } else if (button_False == HIGH) {
        // Resposta errada
        somErro();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Errou!");
        delay(2000);
        fase = 1; // Volta para a primeira fase
        break;
      }
    }
  }
  
  // Fase 4: Pergunta final
  if (fase == 7) {
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Pergunta Final:");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Lua e um planeta?");

    while (true) {
      // Atualiza a leitura dos botões dentro do loop
      button_True = digitalRead(bot1);
      button_False = digitalRead(bot2);

      if (button_False == HIGH) {
        // Resposta correta
        somAcerto();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Venceu o jogo!");
        delay(2000);
        fase = 1; // Reiniciar o jogo
        break;
      } else if (button_True == HIGH) {
        // Resposta errada
        somErro();
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Errou!");
        delay(2000);
        fase = 1; // Reinicia o jogo
        break;
      }
    }
  }
}
