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

uint8_t buttons = lcd.readButtons();

const int sensorPin = A0;

boolean drawPromptDisplayed = false;

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

  

  // setup LEDs
  for(int pinNumber=2; pinNumber<5;pinNumber++){
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
  
}

void loop(){
  if(!drawPromptDisplayed){
    lcd.setCursor(0,0);
    lcd.print(" DRAW SOMETHING");
    lcd.setCursor(0,1);
    lcd.print("WITH YOUR FINGER");
    drawPromptDisplayed == true;
  }
  float sensorVal = 0;
  // Get the currently touched pads
  currtouched = cap.touched();
  
  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
      sensorVal = cap.baselineData - cap.filteredData;
      enableDrawMode();
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
    }
  }

  // reset our state
  lasttouched = currtouched;

  Serial.print("Sensor Value: ");
  Serial.println(sensorVal);

  if(sensorVal < TBD1){
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }

  else if(sensorVal >= TBD1 && sensorVal < TBD2){
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }

  else if(sensorVal >= TBD2 && sensorVal < TBD3){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }

  else if(sensorVal >= TBD3){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
  }
  
  delay(1);
}

void enableDrawMode(){
  for (uint8_t i=0; i<20; i++) {
    if(i%2==0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("   10 SECONDS   ");
      lcd.setCursor(0,0);
      REMAINING"
    }
  }
  lcd.setCursor(0,0);
  lcd.print("");
  lcd.setCursor(0,1);
  lcd.print("WITH YOUR FINGER");
  drawPromptDisplayed == false;
}
