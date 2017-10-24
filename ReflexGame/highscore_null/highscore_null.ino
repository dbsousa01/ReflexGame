#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  int score = 0;
  // put your main code here, to run repeatedly:
  EEPROM.write(0,score);
}
