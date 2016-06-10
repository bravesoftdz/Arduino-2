/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* Crated by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/
#include <LiquidCrystal.h> // includes the LiquidCrystal Library
LiquidCrystal lcd(7, 8, 9, 10, 11 , 12);// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
const int trigPin = 6;
const int echoPin = 5;
long duration;
int distanceCm, distanceInch;
void setup() {
lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
}
void loop() {
  //dc motor setup
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distanceCm= duration*0.034/2;
//distanceInch = duration*0.0133/2;
lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
lcd.print("Distance: "); // Prints string "Distance" on the LCD
lcd.print(distanceCm); // Prints the distance value from the sensor
lcd.print(" cm");
if(distanceCm < 100){
  lcd.print(" ");
}
delay(10);
lcd.setCursor(0,1);
//lcd.print("Distance: ");
//lcd.print(distanceInch);
//lcd.print(" inch");
if(distanceCm > 20){
  forward();
  lcd.print("Forward!!");
} else if(distanceCm > 10){
  backward();
  lcd.print("Backward!!");
} else if(distanceCm > 0){
  stopDC();
  lcd.print("Stop!!    ");
}
delay(10);
}

void forward(){
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
  analogWrite(2, 500);
//  delay(3000);
}

void backward(){
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  analogWrite(2, 300);
//  delay(3000);
}

void stopDC(){
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
//  analogWrite(2, 100);
//  delay(3000);
}
