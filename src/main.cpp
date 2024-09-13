
//#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
//#include <LiquidCrystal_PCF8574.h>
#define COLUMS           20
#define ROWS             4

#define LCD_SPACE_SYMBOL 0x20  //space symbol from the LCD ROM, see p.9 of GDM2004D datasheet

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);


int wheelR = 21.5;
float wheelCM= (wheelR*PI*2);

//int buttonPin=5;
int buttonState=0;
int ledPin = 10;
int hallPin = 7; 


float T = 0.00;
float T2=0.00;
float O = 0.00;
float odo=0;
float rev=0;
float rpm=0;
float loopD=0;
float tripD=0.00;

int print=0;
int mph=0;
int oldtime=0;
int time;
int tripIndex=1;
int tripIndexNew=3;
int odoIndex=0;

void isr() //interrupt service routine
{
 rev++;
 digitalWrite(ledPin, HIGH);
 digitalWrite(ledPin, LOW);
}


void setup() {

  Serial.begin(115200);
  //EEPROM.put(0,0.00); // zeros odometer index
  //EEPROM.put(1,0.00); // zeros trip index

  //EEPROM.get(tripIndex,T);  // keeps trip alive
  //EEPROM.get(odoIndex,O);  // gets old odometer value
 
  odo = O;          // updates odo
  tripD = T;

  while (lcd.begin(COLUMS, ROWS) != 1) //colums - 20, rows - 4
  {
    Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
    delay(5000);   
  }

  lcd.print(F("starting this mf"));    
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);

  pinMode(hallPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  //pinMode(buttonPin, INPUT_PULLUP);
  
  

  attachInterrupt(digitalPinToInterrupt(hallPin), isr, RISING);  //attaching the interrupt
 
}
 

void loop() {

  //int buttonState = digitalRead(buttonPin);
  delay(1000);

  detachInterrupt(digitalPinToInterrupt(hallPin));   
 
  time=millis()-oldtime;        //finds the time 
  rpm=(rev/time)*60000; 
  mph=(wheelCM*rpm)/1056;
  loopD=(rev*wheelCM)/63360;
  tripD=tripD+loopD; 
  
  //EEPROM.put(tripIndex,tripD); 
  //EEPROM.put(odoIndex,(loopD+odo));
  //odo=EEPROM.get(odoIndex,O);
  
  oldtime=millis();       
  rev=0;
  loopD=0;
  //Serial.println(mph);
  //Serial.println(rpm);
  //Serial.println(tripD);
  //Serial.println(wheelCM);
  
  attachInterrupt(digitalPinToInterrupt(hallPin), isr, RISING);
   
  lcd.print(F("MPH: "));
  lcd.setCursor(5,0);
  lcd.print(mph);

  //lcd.setCursor(0,1);
  //lcd.print(F("O: "));
  //lcd.print(odo);
  //lcd.setCursor(8,1);
  //lcd.print(F("T: "));
  //lcd.print(tripD);

  lcd.clear();
  lcd.print(F("MPH: "));
  lcd.setCursor(5,0);
  lcd.print(mph);

 // lcd.setCursor(0,1);
  //lcd.print(F("O: "));
  //lcd.print(odo);
  //lcd.setCursor(8,1);
  //lcd.print(F("T: "));
  //lcd.print(tripD);

 //if (buttonState == LOW) {
    //EEPROM.put(tripIndexNew,5);
    //EEPROM.put(tripIndex,0);
    //tripD=0;
    //digitalWrite(ledPin,HIGH);
    //delay(500);
    //digitalWrite(ledPin,LOW);
  //} 
  
  //for ( int i=0; i<5; i++) {
      //print = EEPROM.get(i,T2);
      //Serial.print(print);
  //}

}