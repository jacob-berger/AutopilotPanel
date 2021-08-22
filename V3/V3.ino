#include <BitsAndDroidsFlightConnector.h>
#include "Button.h"
#include <RotaryEncoder.h>
#include <Adafruit_MCP3008.h>

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;
BitsAndDroidsFlightConnector connector(false);
RotaryEncoder rotary(8, 7);


int count = 0;
int count1 = 0;
int count2 = 0;
int lastState1 = 0; //not pressed
int lastState2 = 0;
int inputThreshold = 800;
Adafruit_MCP3008 adcs[2]; //only expecting 2 for now
int lastStates[16]; //only assuming buttons from adcs
int currentStates[16];
int held[16];
long lastTime;

#define altPin 0
#define spdPin 1
#define hdgPin 2
#define vsPin 3
#define locPin 4
#define appPin 5
#define apPin 6
#define fdPin 7
#define atPin 8
#define swapPin 9

/*
 *  Alt Spd Hdg V/S Loc 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.setTimeout(15);
  while (!Serial);

//  Serial.println("MCP3008 simple test.");

  // Hardware SPI (specify CS, use any available digital)
  // Can use defaults if available, ex: UNO (SS=10) or Huzzah (SS=15)
//  adc.begin();
  // Feather 32u4 (SS=17) or M0 (SS=16), defaults SS not broken out, must specify
  //adc.begin(10);  

  // Software SPI (specify all, use any available digital)
  // (sck, mosi, miso, cs);
  adc1.begin(14, 10, 11, 9);
  adc2.begin(14, 10, 11, 2);
  adcs[0] = adc1;
  adcs[1] = adc2;

  
  
}

void loop() {

//  BITSANDDROIDSCONNECTOR
    connector.dataHandling();

    readInputs();
    
    processInputs();

    bool FDState = connector.getAPFlightDirectorOn();

    digitalWrite(LED_BUILTIN, FDState);

//    delay(1000);
//    
////TO BE MOVED
//    static int pos = 0;
//    rotary.tick();
//
//    int newPos = rotary.getPosition();
//    if (pos != newPos) {
//      Serial.print("pos:");
//      Serial.print(newPos);
//      Serial.print(" dir:");
//      Serial.println((int)(rotary.getDirection()));
//      pos = newPos;
//    }
//
    delay(10);
}

void readInputs() {
  int numADCs = sizeof(adcs)/sizeof(adcs[0]);
  for (int ix = 0; ix < numADCs; ix++) {
    for (int iy = 0; iy < 8; iy++) {
      int value = adcs[ix].readADC(iy);
      int button = (ix * 8) + iy;
      lastStates[button] = currentStates[button];
      if (value > inputThreshold) {
        if (lastStates[button] == 0) {//button pressed
          lastTime = millis();
        }
        currentStates[button] = 1;
      } else {
        currentStates[button] = 0;
        if (lastStates[button] == 1) {//button released
          long current = millis();
//          Serial.println(current - lastTime);
          if ((current - lastTime) > 1000) {
            held[button] = 1;
          }
        }
      }
    }
  }
}

void processInputs() {
  //only send commands for changed states
  for (int ix = 0; ix < sizeof(lastStates); ix++) {
    if (currentStates[ix] < lastStates[ix]) { //only on release
      switch (ix) {
        case altPin:
          if (held[ix] == 1) {//holding for managed
            //not yet implemented
//            Serial.println("Held alt");
            Serial.println(connector.sendAPHeadingHoldOn());
            held[ix] = 0;
          } else {
            Serial.println(connector.sendAPPanelAltitudeHold());
          }
          break;
        case 1:
          if (held[ix] == 1) {//holding for managed
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          } else {
            connector.sendAPAirspeedHold();
          }
          break;
        case hdgPin:
          if (held[ix] == 1) {//holding for managed
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case vsPin:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 3 vs");
          }
          if (held[ix] == 1) {
//            not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case locPin:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 4 loc");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case appPin:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 5 app");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case apPin:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 6 ap");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case fdPin:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 7 fd");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case atPin:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 8 at");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case swapPin:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 9 swap");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case 10:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 10 UNASSIGNED");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case 11:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 11 UNASSIGNED");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case 12:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 12 UNASSIGNED");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case 13:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 13 UNASSIGNED");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case 14:
          if (currentStates[ix] == 1) {
//            Serial.println("Pressed 14 UNASSIGNED");
          }
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          }
          break;
        case 15:
          if (held[ix] == 1) {
            //not yet implemented
//            Serial.println("Held");
            held[ix] = 0;
          } else {
//            Serial.println("Sent FD toggle");
            Serial.println(connector.sendAPFlightDirector());
          }
          break;
      }
    }
  }
}

void displayADCs() {
  for (int chan=0; chan<8; chan++) {
    Serial.print(adc1.readADC(chan)); Serial.print("\t");
  }

  for (int chan=0; chan<8; chan++) {
    Serial.print(adc2.readADC(chan)); Serial.print("\t");
  }

  Serial.print("["); Serial.print(count); Serial.println("]");
  count++;
  
  delay(1000);
}
