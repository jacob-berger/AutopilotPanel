#include <Joystick.h>
#include <BitsAndDroidsFlightConnector.h>
#include "Button.h"
#include <RotaryEncoder.h>
#include <Adafruit_MCP3008.h>

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;
BitsAndDroidsFlightConnector connector(true);
RotaryEncoder spd(2, 3, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder hdg(4, 5);
RotaryEncoder alt(6, 7);
RotaryEncoder vs(8, A0);
Joystick_ Joystick;

#define inputThreshold (uint16_t)800
Adafruit_MCP3008 adcs[2]; //only expecting 2 for now
boolean lastStates[16]; //only assuming buttons from adcs
boolean currentStates[16];
boolean held[16];
boolean isEncoderButton[] = {
                            true, true, true, true,
                            false, false, false, false,
                            false, false, false, false,
                            false, false, false, false
                            };
RotaryEncoder encoders[] = {spd, hdg, alt, vs};
uint16_t lastTime[32];
boolean isAP, isRad;
uint16_t holdTime = 1200;
int pos[4];
int spdIn, hdgIn, altIn, vsIn;
boolean locIn, appIn, apIn, fdIn, atIn;
float com1ActiveIn, com1StandbyIn, com2ActiveIn, com2StandbyIn; 

// Define some constants.
// at 500ms, there should be no acceleration.
constexpr const unsigned long kAccelerationLongCutoffMillis = 500;
// at 4ms, we want to have maximum acceleration
constexpr const unsigned long kAccelerationShortCutffMillis = 8;
// linear acceleration: incline
constexpr static const float m = -0.16;
// linear acceleration: y offset
constexpr static const float c = 84.03;

#define spdPin 0
#define hdgPin 1
#define altPin 2
#define vsPin 3
#define locPin 4
#define appPin 5
#define apPin 6
#define fdPin 7
#define atPin 8
#define swap1Pin 9
#define swap2Pin 10
#define modePin 11

boolean fastPoll = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial);
  
  // (sck, mosi, miso, cs);
  adc1.begin(13, 11, 12, 10);
  adc2.begin(13, 11, 12, 9);
  adcs[0] = adc1;
  adcs[1] = adc2;

//  attachInterrupt(2, rotaryInterrupt, RISING);
  
  Joystick.begin();
}

void loop() {

//  BITSANDDROIDSCONNECTOR
  connector.dataHandling();

//    displayADCs();
//  while (fastPoll) {
//    processEncoders();
//    Serial.println("FastPoll");
//  }
    readInputs();
    processInputs();
    processEncoders();

//    int radIn = connector.getIndicatedAirspeed();
//    Serial.print("Fetched: ");
//    Serial.println(radIn);
  
  delay(10);
}

void readInputs() {
  int numADCs = sizeof(adcs)/sizeof(adcs[0]);
  for (int ix = 0; ix < numADCs; ix++) {
    for (int iy = 0; iy < 8; iy++) {
      int value = adcs[ix].readADC(iy);
      int button = (ix * 8) + iy;
      lastStates[button] = currentStates[button];
      if ((value > inputThreshold && !isEncoderButton[button]) || (value < inputThreshold && isEncoderButton[button])) {
        if (lastStates[button] == false) {//button pressed
          lastTime[button] = millis();
        }
        currentStates[button] = true;
      } else {
        currentStates[button] = false;
        if (lastStates[button] == true) {//button released
          long current = millis();
//          Serial.println(current - lastTime[button]);
          if ((current - lastTime[button]) > 1000) {
            held[button] = true;
          }
        }
      }
    }
  }
}

void processInputs() {
  //only send commands for changed states
  for (int ix = 0; ix < sizeof(lastStates)/sizeof(lastStates[0]); ix++) {
    if ((currentStates[ix] == false) && (lastStates[ix] == true)) { //only on release
      switch (ix) {
        case spdPin:
          if (held[ix] == true) {//holding for managed
            //not yet implemented
            Serial.println("Held spd");
            //TODO
            //include spadnext association with button
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed spd");
            Serial.println(connector.sendAPAirspeedHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case hdgPin:
          if (held[ix] == true) {//holding for managed
            //not yet implemented
            Serial.println("Held hdg");
            //TODO
            //include spadnext association with button
            Joystick.pressButton(ix);  
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed hdg");
            Serial.println(connector.sendAPHeadingHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case altPin:
          if (held[ix] == true) {//holding for managed
            //not yet implemented
            Serial.println("Held alt");
            //TODO
            //include spadnext association with button
//            Joystick.pressButton(ix);
//            delay(50);
//            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed alt");
            Serial.println(connector.sendAPPanelAltitudeHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case vsPin:
          if (held[ix] == true) {
//            not yet implemented
            Serial.println("Held vs");
            //TODO
            //include spadnext association with button
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed vs");
            Serial.println(connector.sendAPVSHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case locPin:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held loc");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed loc");
            Serial.println(connector.sendAPLocHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case appPin:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held app");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed app");
            Serial.println(connector.sendAPAprHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case apPin:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held ap");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed ap");
            Serial.println(connector.sendApMasterOn());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case fdPin:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held fd");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed fd");
            Serial.println(connector.sendAPFlightDirector());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case atPin:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held at");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed at");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case swap1Pin:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held swap1");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed swap1");
//            Serial.println(connector.sendSwapCom1());
            Joystick.pressButton(ix);  
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case swap2Pin:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held swap2");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed swap2");
//            Serial.println(connector.sendSwapCom2());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case modePin:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held mode");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            isAP = !isAP;
            if (isAP) {
              Serial.println("AP MODE");
            } else {
              Serial.println("RAD MODE");
            }
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 12:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held 12");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 13:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held 13");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed 13");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 14:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held 14");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed 14");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 15:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held 15");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed 15");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
      }
    }
  }
}

void processEncoders() {
  boolean changed, timedOut = false;
  long timeout = 2000;
  long start = millis();
  do {
    for (int ix = 0; ix < sizeof(encoders)/sizeof(encoders[0]); ix++) {
    encoders[ix].tick();

    int newPos = encoders[ix].getPosition();
    if (pos[ix] != newPos) {
      changed = true;
      start = millis();
      long difference = millis() - lastTime[ix];
      int dir = (int)encoders[ix].getDirection();
      if (dir > 0) {
        switch (ix) {
          case 0:
            if (isAP) {
              Serial.println(connector.sendAPSpeedInc());
            } else {
              Serial.println(connector.sendCom1WholeInc());
            }
            break;
          case 1:
            if (isAP) {
              Serial.println(connector.sendHeadingBugInc());
            } else {
              Serial.println(connector.sendCom1FractInc());
            }
            break;
          case 2:
            if (isAP) {
              Serial.println(connector.sendAPAltitudeInc());
            } else {
              Serial.println(connector.sendCom2WholeInc());
            }
            break;
          case 3:
            if (isAP) {
              Serial.println(connector.sendAPVSInc());
            } else {
              Serial.println(connector.sendCom2FractInc());
            }
            break;
        }
        Serial.println(newPos);
        Serial.println(pos[ix]);
        lastTime[ix] = millis();
        pos[ix] = newPos;
      } else if (dir < 0) {
        switch (ix) {
          case 0:
            if (isAP) {
              Serial.println(connector.sendAPSpeedDec());
            } else {
              Serial.println(connector.sendCom1WholeDec());
            }
            break;
          case 1:
            if (isAP) {
              Serial.println(connector.sendHeadingBugDec());
            } else {
              Serial.println(connector.sendCom1FractDecr());
            }
            break;
          case 2:
            if (isAP) {
              Serial.println(connector.sendAPAltitudeDec());
            } else {
              Serial.println(connector.sendCom2WholeDec());
            }
            break;
          case 3:
            if (isAP) {
              Serial.println(connector.sendAPVSDec());
            } else {
              Serial.println(connector.sendCom2FractDecr());
            }
            break;
        }
        Serial.println(newPos);
        Serial.println(pos[ix]);
        lastTime[ix] = millis();
        pos[ix] = newPos;
        }
      } else {
        changed = false;
      }

//      int newPos = encoders[ix].getPosition();
//      if (pos[ix] != newPos) {
//        Serial.print("pos ");
//        Serial.print(ix);
//        Serial.print(": ");
//        Serial.print(newPos);
//        Serial.print(" dir:");
//        Serial.println((int)(encoders[ix].getDirection()));
//        pos[ix] = newPos;
//      }
    }
    timedOut = (millis() - start < 2000);
  } while (!timedOut && changed);
//  return changed;
}

void displayADCs() {
  for (int chan=0; chan<8; chan++) {
    Serial.print(adc1.readADC(chan)); Serial.print("\t");
  }

  for (int chan=0; chan<8; chan++) {
    Serial.print(adc2.readADC(chan)); Serial.print("\t");
  }

  Serial.println();
  
  delay(1000);
}
