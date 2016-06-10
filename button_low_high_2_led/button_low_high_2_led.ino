
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin1 = 4;
const int buttonPin2 = 3;
const int buttonPin3 = 2;

const int ledPin1 = 13;
const int ledPin2 = 12;
const int ledPin3 = 11;


// variables will change:
// variable for reading the pushbutton status
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

int lastButton1State = 0;
int lastButton2State = 0;
int lastButton3State = 0;

int pushCountButton1 = 0;
int pushCountButton2 = 0;
int pushCountButton3 = 0;

boolean onOffLed1 = false;
boolean onOffLed2 = false;
boolean onOffLed3 = false;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
}

void loop() {
  
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);

  if(buttonState1 != lastButton1State){
    if (buttonState1 == HIGH) {
      pushCountButton1++;
    }
    delay(50);
  }
  lastButton1State = buttonState1;
  
  if(buttonState2 != lastButton2State){
    if (buttonState2 == HIGH) {
      pushCountButton2++;
    }
    delay(50);
  }
  lastButton2State = buttonState2;
  
  if(buttonState3 != lastButton3State){
    if (buttonState1 == HIGH) {
      pushCountButton3++;
    }
    delay(50);
  }
  lastButton3State = buttonState3;
  
  if (pushCountButton1 % 2 == 0) {
    onOffLed1 = true;
    onOffLed2 = false;
    onOffLed3 = false;
  }else{
    onOffLed1 = false;
  } 

  if (pushCountButton2 % 2 == 0) {
    onOffLed1 = false;
    onOffLed2 = true;
    onOffLed3 = false;
  }else{
    onOffLed2 = false;
  } 

  if (pushCountButton3 % 2 == 0) {
    onOffLed1 = false;
    onOffLed2 = false;
    onOffLed3 = true;
  } else {
    onOffLed3 = false;
  }  

  if (onOffLed1 == true) {
      turnOnLED(ledPin1, 300);      
  }
  if (onOffLed2 == true) {
      turnOnLED(ledPin2, 200);
  }
  if (onOffLed3 == true) {
      turnOnLED(ledPin3, 100);      
  }
  
}

void turnOnLED(int led, int d){
    digitalWrite(led, HIGH);
    delay(d);
    digitalWrite(led, LOW);
    delay(d);
}

