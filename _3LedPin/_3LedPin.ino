const int ledPin1 = 12;
const int ledPin2 = 10;
const int ledPin3 = 9;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

}

void loop() {
 turnOnLED(ledPin1, 100);
 turnOnLED(ledPin2, 200);
 turnOnLED(ledPin3, 300);

}

void turnOnLED(int led, int d){
    digitalWrite(led, HIGH);
    delay(d);
    digitalWrite(led, LOW);
    delay(d);
}
