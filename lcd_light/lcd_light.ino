#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11 , 12);
String str; 

void setup() { 
  lcd.begin(16, 2);
  lcd.setCursor(0,1);
  lcd.write("LIGHT: ");
}

void loop() { 

  int lightIntensity  = analogRead(A0);
  lightIntensity = map(lightIntensity, 1050, 20, 0, 100);
  str = String(lightIntensity)+"%";
  
  lcd.setCursor(7,1);
  lcd.print(str);
  delay(100);
  
}
