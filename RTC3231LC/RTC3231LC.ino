//#include "DHT.h"
#include <Wire.h>
#include "ds3231.h"
#include <LiquidCrystal.h>


LiquidCrystal lcd(7, 8, 9, 10, 11 , 12);

#define BUFF_MAX 128
#define DHTTYPE DHT11
//#define DHTPIN 4

//DHT dht(DHTPIN, DHTTYPE);

uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 1000;
int buzzpin = 6;
int relay = 5;
boolean relayIsOn = false;
int relayState = 0;
int lastRelayState = 0;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    DS3231_init(DS3231_INTCN);
    memset(recv, 0, BUFF_MAX);
    Serial.println("GET time");
    lcd.begin(16, 2);
    lcd.clear();
    
    //Serial.println("Setting time");
    //parse_cmd("T003110729052016",16);
    pinMode(buzzpin, OUTPUT);
    buzz(200);
    buzz(200);
    buzz(200);

    pinMode(relay, OUTPUT);
//    relayOff();
//    relayOn();
//    dht.begin();
}

void loop()
{
    char in;
    char tempF[6]; 
    float temperature;
    char buff[BUFF_MAX];
    unsigned long now = millis();
    struct ts t;
//    float celcius = dht.readTemperature();
//    float humidity = dht.readHumidity();

    // show time once in a while
    if ((now - prev > interval) && (Serial.available() <= 0)) {
        DS3231_get(&t); //Get time
        parse_cmd("C",1);
        temperature = DS3231_get_treg(); //Get temperature
        dtostrf(temperature, 5, 1, tempF);

        lcd.clear();
        lcd.setCursor(0,0);
        
        lcd.print(t.mday);
        
        printMonth(t.mon);
        
        lcd.print(t.year);
        
        lcd.setCursor(0,1); //Go to second line of the LCD Screen
        lcd.print(t.hour);
        lcd.print(":");
        if(t.min<10)
        {
          lcd.print("0");
        }
        lcd.print(t.min);
        lcd.print(":");
        if(t.sec<10)
        {
          lcd.print("0");
        }
        lcd.print(t.sec);

        
        lcd.print(' ');
        lcd.print(tempF);
        lcd.print((char)223);
        lcd.print("C ");
//        Serial.print("Temperature = ");
//        Serial.println(celcius);
//        Serial.print("Humidity = ");
//        Serial.println(humidity);
//    
//        lcd.print(' ');
//        lcd.print(celcius);
//        lcd.print((char)223);
//        lcd.print("C ");
        prev = now;
    }

    
    if (Serial.available() > 0) {
        in = Serial.read();

        if ((in == 10 || in == 13) && (recv_size > 0)) {
            parse_cmd(recv, recv_size);
            recv_size = 0;
            recv[0] = 0;
        } else if (in < 48 || in > 122) {;       // ignore ~[0-9A-Za-z]
        } else if (recv_size > BUFF_MAX - 2) {   // drop lines that are too long
            // drop
            recv_size = 0;
            recv[0] = 0;
        } else if (recv_size < BUFF_MAX - 2) {
            recv[recv_size] = in;
            recv[recv_size + 1] = 0;
            recv_size += 1;
        }

    }
    
    if(t.hour == 3){
      if(t.min > 45 && t.min < 50){
        buzz(100);
        lcd.setCursor(0,1);
        lcd.print("BANGUN SAHUR!!!!");
        delay(500);
      }
    }
   
    if(relayIsOn == true){
      relayState = 1;
    } else {
      relayState = 0;
    }
    
    if(relayState != lastRelayState){
      if(relayState == 1){
        Serial.println("relay on");
      } else {
        Serial.println("relay off");
      }
    }  
      if((int)temperature < 29){
        //Serial.println("temp < 29");
        if(relayState != lastRelayState){
          if(relayState == 1){
              lcd.setCursor(0,1);
              lcd.print("Matiin Kipas");
              delay(200);
          }
        }else{
          if(relayState == 1){
              lcd.setCursor(0,1);
              lcd.print("Matiin Kipas");
              relayOff();
              delay(200);
          }
        }
        
      } else {
        //Serial.println("temp > 29");
        if(relayState != lastRelayState){
          if(relayState == 0){
              lcd.setCursor(0,1);
              lcd.print("Hidupin Kipas");
              relayOn();
              delay(200);
          }
        }else{
          if(relayState == 0){
              lcd.setCursor(0,1);
              lcd.print("Hidupin Kipas");
              relayOn();
              delay(200);
          }
        }
        
     }
    
  lastRelayState = relayState;
}

void parse_cmd(char *cmd, int cmdsize)
{
    uint8_t i;
    uint8_t reg_val;
    char buff[BUFF_MAX];
    struct ts t;

    //snprintf(buff, BUFF_MAX, "cmd was '%s' %d\n", cmd, cmdsize);
    //Serial.print(buff);

    // TssmmhhWDDMMYYYY aka set time
    if (cmd[0] == 84 && cmdsize == 16) {
        //T355720619112011
        t.sec = inp2toi(cmd, 1);
        t.min = inp2toi(cmd, 3);
        t.hour = inp2toi(cmd, 5);
        t.wday = inp2toi(cmd, 7);
        t.mday = inp2toi(cmd, 8);
        t.mon = inp2toi(cmd, 10);
        t.year = inp2toi(cmd, 12) * 100 + inp2toi(cmd, 14);
        DS3231_set(t);
        Serial.println("OK");
    } else if (cmd[0] == 49 && cmdsize == 1) {  // "1" get alarm 1
        DS3231_get_a1(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 50 && cmdsize == 1) {  // "2" get alarm 1
        DS3231_get_a2(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 51 && cmdsize == 1) {  // "3" get aging register
        Serial.print("aging reg is ");
        Serial.println(DS3231_get_aging(), DEC);
    } else if (cmd[0] == 65 && cmdsize == 9) {  // "A" set alarm 1
        DS3231_set_creg(DS3231_INTCN | DS3231_A1IE);
        //ASSMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // ss, mm, hh, dd
        }
        byte flags[5] = { 0, 0, 0, 0, 0 };
        DS3231_set_a1(time[0], time[1], time[2], time[3], flags);
        DS3231_get_a1(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 66 && cmdsize == 7) {  // "B" Set Alarm 2
        DS3231_set_creg(DS3231_INTCN | DS3231_A2IE);
        //BMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // mm, hh, dd
        }
        byte flags[5] = { 0, 0, 0, 0 };
        DS3231_set_a2(time[0], time[1], time[2], flags);
        DS3231_get_a2(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 67 && cmdsize == 1) {  // "C" - get temperature register
        Serial.print("temperature reg is ");
        Serial.println(DS3231_get_treg(), DEC);
    } else if (cmd[0] == 68 && cmdsize == 1) {  // "D" - reset status register alarm flags
        reg_val = DS3231_get_sreg();
        reg_val &= B11111100;
        DS3231_set_sreg(reg_val);
    } else if (cmd[0] == 70 && cmdsize == 1) {  // "F" - custom fct
        reg_val = DS3231_get_addr(0x5);
        Serial.print("orig ");
        Serial.print(reg_val,DEC);
        Serial.print("month is ");
        Serial.println(bcdtodec(reg_val & 0x1F),DEC);
    } else if (cmd[0] == 71 && cmdsize == 1) {  // "G" - set aging status register
        DS3231_set_aging(0);
    } else if (cmd[0] == 83 && cmdsize == 1) {  // "S" - get status register
        Serial.print("status reg is ");
        Serial.println(DS3231_get_sreg(), DEC);
    } else {
        Serial.print("unknown command prefix ");
        Serial.println(cmd[0]);
        Serial.println(cmd[0], DEC);
    }
}

void printMonth(int month)
{
  switch(month)
  {
    case 1: lcd.print(" January ");break;
    case 2: lcd.print(" February ");break;
    case 3: lcd.print(" March ");break;
    case 4: lcd.print(" April ");break;
    case 5: lcd.print(" May ");break;
    case 6: lcd.print(" June ");break;
    case 7: lcd.print(" July ");break;
    case 8: lcd.print(" August ");break;
    case 9: lcd.print(" September ");break;
    case 10: lcd.print(" October ");break;
    case 11: lcd.print(" November ");break;
    case 12: lcd.print(" December ");break;
    default: lcd.print(" Error ");break;
  } 
}

void buzz(unsigned char t){
  analogWrite(buzzpin, 170);
  delay(t);
  analogWrite(buzzpin, 0);
  delay(t);
}

void relayOn(){
  relayIsOn = true;
  digitalWrite(relay, LOW);  // turn on relay
}

void relayOff(){
  relayIsOn = false;
  digitalWrite(relay, HIGH);   // turn off relay
}

