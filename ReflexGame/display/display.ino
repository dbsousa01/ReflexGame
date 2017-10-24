// Programa : Display LCD 16x2 e modulo I2C
// Autor : Arduino e Cia

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);
 
void setup()
{
 lcd.begin (16,2);
  lcd.setBacklight(HIGH);
  
 
}
 
void loop()
{
  lcd.setCursor(0,0);
  lcd.print("Arduino e Cia !!");
  lcd.setCursor(0,1);
  
   lcd.print("LCD e modulo I2C");
  lcd.clear();
}

