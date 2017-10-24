#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"
 
#define LED1 5
#define LED2 4
#define LED3 3
#define LED4 2
#define botao1 13
#define botao2 12
#define botao3 11
#define botao4 10
#define buzzer 1

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned int score;
unsigned int highscore;
void setup() {
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(botao4, INPUT);

  // initialize the LCD
  lcd.begin(16,2);

  // Turn on the blacklight and print a message.
  lcd.setBacklight(HIGH);
}

void start_routine()    //Three blink routine of the leftmost LED to indicate the start of the game
{
  int freq_tune = 200;
  
  for (int i = 0; i <= 2; i++)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);// TO DO: play a sound when the leds blink
    tone(buzzer, freq_tune + 100); //initial 3 tunes to start the game
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    noTone(buzzer);
    delay(300);
  }
  Serial.println("GO"); //TO DO: (write to the display - ?)
}

void loop() {
  //TO DO: Read the highscore written in the eeprom.read()
  int tolerancia = 2000;
  int tempo_inicio_ronda;
  int onLED;
  int lost = 0;
  int passou = 0;
  int i;

  highscore = EEPROM.read(0);

  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  
  lcd.print("HIGHSCORE:");
  lcd.setCursor(11,0);
  lcd.print(highscore);
  lcd.setCursor(7,1);
  lcd.print("PONTOS:");
  lcd.setCursor(14,1);
  lcd.print(score);
  start_routine();
  while (lost == 0){
    //POR CAUSA DESTE DELAY, PODE-SE CARREGAR EM QUALQUER BOTAO, ENTRE BOTOES
    delay(1000);
    onLED = random(LED4, LED1 + 1);
    tempo_inicio_ronda = millis();
         
    
    while (millis() - tempo_inicio_ronda <= tolerancia && passou == 0 && lost == 0){
      digitalWrite(onLED, HIGH);
      if(onLED == LED1)
        tone(buzzer, 261.63);
      if(onLED == LED2)
        tone(buzzer, 293.66);
      if(onLED == LED3)
        tone(buzzer,329.63);
      if(onLED == LED4)
        tone(buzzer, 349.23);
      if (digitalRead((onLED + 8)) == HIGH) {
        passou = 1;
        noTone(buzzer);
        break;
      }else if(passou == 0){
        if (onLED == LED1) {                        //verifica se foram pressionados outros butões, está a dar bug
          if (digitalRead(botao2) == HIGH || digitalRead(botao3) == HIGH || digitalRead(botao4) == HIGH) {
            lost = 1;
            break;
          }
        }else if (onLED == LED2) {
          if (digitalRead(botao1) == HIGH || digitalRead(botao3) == HIGH || digitalRead(botao4) == HIGH) {
            lost = 1;
            break;
          }
        }else if (onLED == LED3) {
          if (digitalRead(botao1) == HIGH || digitalRead(botao2) == HIGH || digitalRead(botao4) == HIGH) {
            lost = 1;
            break;
          }
        }else if (onLED == LED4) {
          if (digitalRead(botao1) == HIGH || digitalRead(botao2) == HIGH || digitalRead(botao3) == HIGH) {
            lost = 1;
            break;
          }
        }
      }
    }
    if(passou == 0){
      lost = 1;
      Serial.println("perdeste");
      digitalWrite(onLED, LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("GAME OVER!");
      lcd.setCursor(0,1);
      lcd.print("Fizeste");
      lcd.setCursor(8,1);
      lcd.print(score);
      lcd.setCursor(10,1);
      lcd.print("pontos!");
      delay(4000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("HIGHSCORE:");
      lcd.setCursor(11,0);
      lcd.print(highscore);

      digitalWrite(onLED, LOW);
      noTone(buzzer);
      tone(buzzer, 700); // play a sound when he loses?
      delay(1000);
      noTone(buzzer);
      if(score > highscore){
        Serial.println("New Highscore");
        lcd.setCursor(0,1);
        lcd.print("New Highscore!");
        EEPROM.write(0,score);
      }
      //reset highscore
      delay(3000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("HIGHSCORE:");
      lcd.setCursor(11,0);
      lcd.print(highscore);

      while(1) {}; // infinity cycle since the game is over, you can start it again by pressing the reset button
    }else{
      Serial.println("+1 ponto");
      passou = 0;
      lcd.setCursor(0,1);
      //Serial.println("+1ponto");
      ++score;
      lcd.print("+1 !");
      lcd.setCursor(14,1);
      lcd.print(score);
      tolerancia = tolerancia - 100;
      noTone(buzzer);
    }
    digitalWrite(onLED, LOW);
  }
}


