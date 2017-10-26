#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"
 
#define LED1 5
#define LED2 4
#define LED3 3
#define LED4 2
#define botao1 9
#define botao2 8
#define botao3 7
#define botao4 6
#define buzzer 13

int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
  };
 int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12
  };
  
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned int score;
unsigned int highscore;
int song = 0; 
void(* resetFunc) (void) = 0;
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
  pinMode(buzzer, OUTPUT);

  // initialize the LCD
  lcd.begin(16,2);
  sing(1);
  // Turn on the blacklight and print a message.
  lcd.setBacklight(HIGH);

 /* while(1){
    tone(buzzer,NOTE_G6);
    delay(500);
    tone(buzzer,NOTE_A6);
   
    delay(500);
    tone(buzzer,NOTE_B6);
    delay(500);
    tone(buzzer,NOTE_C7);
    }*/
}
    
  
void sing(int s) {
  // iterate over the notes of the melody:
  song = s;
  if (s==1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("READY?");
    lcd.setCursor(0,1);
    lcd.print("SET? GO!");
    delay(500);
 
    //Serial.println(" 'Mario Theme'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
      
      
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];
 
      buzz(buzzer, melody[thisNote], noteDuration);
      //lcd.setCursor(0,0);
      //lcd.print(melody[thisNote]);
      
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
 
      // stop the tone playing:
      buzz(buzzer, 0, noteDuration);
 
    }
  }
}
 
void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  
 
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);}
 

void start_routine()    //Three blink routine of the leftmost LED to indicate the start of the game
{
  //int freq_tune = 300;
  
  for (int i = 0; i <= 2; i++)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);// TO DO: play a sound when the leds blink
    // tone(buzzer, freq_tune + 10); //initial 3 tunes to start the game
    //delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    //noTone(buzzer);
    //delay(300);
  }
  Serial.println("GO"); //TO DO: (write to the display - ?)
}
void loop() {
  //TO DO: Read the highscore written in the eeprom.read()
  double tolerancia = 3000;
  double tempo_inicio_ronda;
  int onLED;
  int lost = 0;
  int passou = 0;
  int i;

  highscore = EEPROM.read(0);

  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("HIGHSCORE:");
  lcd.setCursor(11,0);
  lcd.print(highscore);
  //lcd.setCursor(7,1);
  
  //lcd.print("PONTOS:");
  lcd.setCursor(14,1);
  lcd.print(score);
  start_routine();
  while (lost == 0){
    //POR CAUSA DESTE DELAY, PODE-SE CARREGAR EM QUALQUER BOTAO, ENTRE BOTOES
    delay(1000);
    onLED = random(LED4, LED1 + 1);
    tempo_inicio_ronda = millis();
  
  //lcd.clear();
  //Serial.println(double(millis()-tempo_inicio_ronda));       
    
    while (millis() - tempo_inicio_ronda <= tolerancia && passou == 0 && lost == 0){
      digitalWrite(onLED, HIGH);
      if(onLED == LED1)
        tone(buzzer, NOTE_G6);
        //noTone(buzzer);
      if(onLED == LED2)
        tone(buzzer,  NOTE_A6 );
        //noTone(buzzer);
      if(onLED == LED3)
        tone(buzzer, NOTE_B6);
        //noTone(buzzer);
      if(onLED == LED4)
        tone(buzzer, NOTE_C7);
        //noTone(buzzer);
      if (digitalRead((onLED + 4)) == HIGH) {
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

     /* tone(buzzer,2200); // then buzz by going high
      tone(buzzer,1000);
      tone(buzzer,500);
      tone(buzzer,200);
      tone(buzzer,500);
      delayMicroseconds(100);    // waiting
      noTone(buzzer);  // going low
      delayMicroseconds(100);    // and waiting more
      tone(buzzer,2200);*/ 
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
      
      /*noTone(buzzer);
      tone(buzzer, 700); // play a sound when he loses?
      delay(500);
      noTone(buzzer);
      */
      //delay(4000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("HIGHSCORE:");
      lcd.setCursor(11,0);
      lcd.print(highscore);

      digitalWrite(onLED, LOW);
      
      if(score > highscore){
          highscore = score; 
          Serial.println("New Highscore");
          lcd.setCursor(0,1);
          lcd.print("New Highscore!");
          EEPROM.write(0,score);
          //reset highscore
          delay(3000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("HIGHSCORE:");
          lcd.setCursor(11,0);
          lcd.print(highscore);
        }else{
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Your Score:");
          lcd.setCursor(11,0);
          lcd.print(score);
          lcd.setCursor(0,1);
          lcd.print("Click RED&BLUE!");
                    
        }
      }
      else{
      //Serial.println("+1 ponto");
      passou = 0;
      lcd.setCursor(0,1);
      //Serial.println("+1ponto");
      ++score;
      lcd.print(tolerancia);
      lcd.setCursor(5,1);
      
      lcd.setCursor(14,1);
      lcd.print(score);
      //faste
      tolerancia = tolerancia - random(-50,150);
      noTone(buzzer);
    }
    digitalWrite(onLED, LOW);

  }
  while(1) {
  if(digitalRead(botao1) ==  HIGH && digitalRead(botao4) ==  HIGH){
    resetFunc();
    }
  // infinity cycle since the game is over, you can start it again by pressing the reset button
  }
}

