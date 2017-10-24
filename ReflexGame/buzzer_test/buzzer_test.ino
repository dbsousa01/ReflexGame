const int buzzer = 1; //buzzer to arduino pin 9


void setup(){
 
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

}

void loop(){
 
  tone(buzzer, 4000); 
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
  
}
