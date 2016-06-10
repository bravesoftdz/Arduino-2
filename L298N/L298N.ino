void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(9, 200);
  delay(1000);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  analogWrite(9, 100);
  delay(1000);
}
