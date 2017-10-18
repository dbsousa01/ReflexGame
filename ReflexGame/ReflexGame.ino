#define LED1 3
#define LED2 4
#define LED3 5
#define LED4 6
#define botao 2

unsigned int points;
void setup() {
Serial.begin(9600);

pinMode(LED1, OUTPUT); 
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
pinMode(LED4, OUTPUT);
pinMode(botao, INPUT);
attachInterrupt(digitalPinToInterrupt(botao), point_count, RISING);      //Setting the external interrupt on pin 2 raising edge
}

void start_routine()    //Three blink routine of the leftmost LED to indicate the start of the game
{
 for(int i=0;i<=2;i++)
 {
  digitalWrite(3,HIGH); // TO DO: play a sound when the leds blink
  delay(500);
  digitalWrite(3,LOW);
  delay(500);
 }
 Serial.println("GO"); //TO DO: (write to the display - ?)
}

void point_count()              //Counting points when interrupt encounters
{
 digitalWrite(LED1,LOW); //put a low the on pin
 digitalWrite(LED2,LOW); //put a low the on pin
 digitalWrite(LED3,LOW); //put a low the on pin
 digitalWrite(LED4,LOW); //put a low the on pin
 points=points+1;
}

void loop() {
  //TO DO: Read the highscore written in the eeprom.read()
  start_routine();
  int threshold = 100;
  int timeLeft = 2000;
  int LED;
  int lost = 0;

  while(lost==0){
    onLED = random(LED1,LED4)
    digitalWrite(onLED,HIGH); //Turns on a random LED TO DO: play a sound when the led is on
    timeStart = millis(); //Takes the time when the LED turns on
    while(1){
      if(millis() - timeStart <= timeLeft && digitalRead(onLED)==1){ //If the time is out and the LED is still on you lose
        lost = 1;
        Serial.println("Game Over") // TO DO: Print this in the display.
        break;
      }
    }
    timeLeft = timeLeft - threshold; //Makes the game harder by reducing the play time - time fucntion could change
  }
  
  Serial.println(points); //TO DO: Write to the display the score and update the higscore if the case, eeprom.write().
  while(1){} //infinite cycle since the game is over, it can start again by pressing the arduino reset button.
}
