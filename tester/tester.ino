#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

Adafruit_MPR121 cap = Adafruit_MPR121();

uint16_t lasttouched = 0;
uint16_t currtouched = 0;

//uint8_t buttons = lcd.readButtons();

boolean drawPromptDisplayed = false;
boolean exitDrawMode = false;
boolean displayResults = false;

int time = 0;
float sensorVal = 0;

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);

  while (!Serial) {
    delay(10);
  }

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
//
//  
//
//  // setup LEDs
//  for(int pinNumber=2; pinNumber<5;pinNumber++){
//    pinMode(pinNumber, OUTPUT);
//    digitalWrite(pinNumber, LOW);
//  }
  
}

void loop(){
  if(displayResults){
    processResults();
  }
  if(!drawPromptDisplayed){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" DRAW SOMETHING");
    lcd.setCursor(0,1);
    lcd.print("WITH YOUR FINGER");
    drawPromptDisplayed = true;
  }
  
  // Get the currently touched pads
  currtouched = cap.touched();
  
  checkForTouch();
  // reset our state
  lasttouched = currtouched;
  
  delay(1);
}

void checkForTouch(){
  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
      sensorVal = cap.baselineData(i) - cap.filteredData(i);
      if(drawPromptDisplayed){
        enableDrawMode();
      }
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
//      if(!drawPromptDisplayed){
//        exitDrawMode = true;
//      }
    }
  }
}

void enableDrawMode(){
  drawPromptDisplayed = false;
  for (uint8_t i=0; i<10; i++) {
//    if(i==0 || i==3 || i==6 || i==9 || i==12 || i=15 || i==18){
//      digitalWrite(2, HIGH);
//      digitalWrite(3, LOW);
//      digitalWrite(4, LOW);
//    }
//    else if(i==1 || i==4 || i==7 || i==10 || i==13 || i=16 || i==19){
//      digitalWrite(2, LOW);
//      digitalWrite(3, HIGH);
//      digitalWrite(4, LOW);
//    }
//    else if(i==2 || i==5 || i==8 || i==11 || i==14 || i=17 || i==20){
//      digitalWrite(2, LOW);
//      digitalWrite(3, LOW);
//      digitalWrite(4, HIGH);
//    }
    if(i%2==0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("   ");
      time = (10-i)/2;
      lcd.print(time);
      lcd.print(" SECONDS   ");
      lcd.setCursor(0,1);
      lcd.print("   REMAINING   ");
    }
//    checkForTouch();
//    if(exitDrawMode){
//      displayResults = true;
//      exit(0);
//    }
    delay(500);
  }
  displayResults = true;
}


void processResults(){
  displayResults = false;
  Serial.print(sensorVal);
  
  if(sensorVal < 15){
    lcd.clear();
    lcd.print(" COLD FISH: GO ");
    lcd.setCursor(0,1);
    lcd.print("FIND INSPIRATION");
    delay(1000);
//    digitalWrite(2, LOW);
//    digitalWrite(3, LOW);
//    digitalWrite(4, LOW);
  }

  else if(sensorVal >= 15 && sensorVal < 30){
    lcd.clear();
    lcd.print("  LUKEWARM: DO  ");
    lcd.setCursor(0,1);
    lcd.print("10 JUMPING JACKS");
    delay(1000);
//    digitalWrite(2, HIGH);
//    digitalWrite(3, LOW);
//    digitalWrite(4, LOW);
  }

  else if(sensorVal >= 30 && sensorVal < 45){
    lcd.clear();
    lcd.print("  IN THE ZONE:  ");
    lcd.setCursor(0,1);
    lcd.print("  GET TO WORK  ");
    delay(1000);
//    digitalWrite(2, HIGH);
//    digitalWrite(3, HIGH);
//    digitalWrite(4, LOW);
  }

  else if(sensorVal >= 45){
    lcd.clear();
    lcd.print("    ON FIRE:    ");
    lcd.setCursor(0,1);
    lcd.print(" MAKE COOL SHIT ");
    delay(2000);
//    digitalWrite(2, HIGH);
//    digitalWrite(3, HIGH);
//    digitalWrite(4, HIGH);
  }
}
