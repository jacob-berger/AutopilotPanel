#include <Joystick.h>
#include <BitsAndDroidsFlightConnector.h>
#include "Button.h"
#include <RotaryEncoder.h>
#include <Adafruit_MCP3008.h>

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;
BitsAndDroidsFlightConnector connector(true);
RotaryEncoder spd(2, 3);
RotaryEncoder hdg(4, 5);
RotaryEncoder alt(6, 7);
RotaryEncoder vs(8, A0);
Joystick_ Joystick;

static uint8_t inputThreshold = 800;
Adafruit_MCP3008 adcs[2]; //only expecting 2 for now
int lastStates[16]; //only assuming buttons from adcs
int currentStates[16];
int held[16];
boolean isEncoderButton[] = {
                            true, true, true, true,
                            false, false, false, false,
                            false, false, false, false,
                            false, false, false, false
                            };
RotaryEncoder encoders[] = {spd, hdg, alt, vs};
long lastTime[32];
boolean isAP, isRad;
long holdTime = 1200;
static int pos[4];
int spdIn, hdgIn, altIn, vsIn, locIn, appIn, apIn, fdIn, atIn;
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

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
//  Serial.begin(115200);
//  Serial.setTimeout(15);
  while (!Serial);

//  Serial.println("MCP3008 simple test.");

  // Hardware SPI (specify CS, use any available digital)
  // Can use defaults if available, ex: UNO (SS=10) or Huzzah (SS=15)
//  adc.begin();
  // Feather 32u4 (SS=17) or M0 (SS=16), defaults SS not broken out, must specify
  //adc.begin(10);  

  // Software SPI (specify all, use any available digital)
  // (sck, mosi, miso, cs);
  adc1.begin(13, 11, 12, 10);
  adc2.begin(13, 11, 12, 9);
  adcs[0] = adc1;
  adcs[1] = adc2;
  
  Joystick.begin();
}

void loop() {

//  BITSANDDROIDSCONNECTOR
    connector.dataHandling();

//    displayADCs();

    readInputs();
    processInputs();
//    processEncoders();

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
    delay(100);
}

void retrieveVars() {
//  spdIn = ?????? Maybe use simconnect
    hdgIn = connector.getApHeadingLock();
    Serial.print("Heading: ");
    Serial.print(hdgIn);
    Serial.println(".");
}

void readInputs() {
  int numADCs = sizeof(adcs)/sizeof(adcs[0]);
  for (int ix = 0; ix < numADCs; ix++) {
    for (int iy = 0; iy < 8; iy++) {
      int value = adcs[ix].readADC(iy);
      int button = (ix * 8) + iy;
      lastStates[button] = currentStates[button];
      if ((value > inputThreshold && isEncoderButton[button] == false) || (value < inputThreshold && isEncoderButton[button] == true)) {
        if (lastStates[button] == 0) {//button pressed
          lastTime[button] = millis();
        }
        currentStates[button] = 1;
      } else {
        currentStates[button] = 0;
        if (lastStates[button] == 1) {//button released
          long current = millis();
//          Serial.println(current - lastTime[button]);
          if ((current - lastTime[button]) > 1000) {
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
        case spdPin:
          if (held[ix] == 1) {//holding for managed
            //not yet implemented
            Serial.println("Held spd");
            //TODO
            //include spadnext association with button
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed spd");
            Serial.println(connector.sendAPAirspeedHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case hdgPin:
          if (held[ix] == 1) {//holding for managed
            //not yet implemented
            Serial.println("Held hdg");
            //TODO
            //include spadnext association with button
            Joystick.pressButton(ix);  
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed hdg");
            Serial.println(connector.sendAPHeadingHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case altPin:
          if (held[ix] == 1) {//holding for managed
            //not yet implemented
            Serial.println("Held alt");
            //TODO
            //include spadnext association with button
//            Joystick.pressButton(ix);
//            delay(50);
//            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed alt");
            Serial.println(connector.sendAPPanelAltitudeHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case vsPin:
          if (held[ix] == 1) {
//            not yet implemented
            Serial.println("Held vs");
            //TODO
            //include spadnext association with button
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed vs");
            Serial.println(connector.sendAPVSHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case locPin:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held loc");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed loc");
            Serial.println(connector.sendAPLocHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case appPin:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held app");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed app");
            Serial.println(connector.sendAPAprHold());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case apPin:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held ap");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed ap");
            Serial.println(connector.sendApMasterOn());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case fdPin:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held fd");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed fd");
            Serial.println(connector.sendAPFlightDirector());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case atPin:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held at");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed at");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case swap1Pin:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held swap1");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed swap1");
//            Serial.println(connector.sendSwapCom1());
            Joystick.pressButton(ix);  
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case swap2Pin:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held swap2");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed swap2");
//            Serial.println(connector.sendSwapCom2());
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case modePin:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held mode");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
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
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held 12");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 13:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held 13");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed 13");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 14:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held 14");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
          } else {
            Serial.println("Pressed 14");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 15:
          if (held[ix] == 1) {
            //not yet implemented
            Serial.println("Held 15");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = 0;
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
  for (int ix = 0; ix < sizeof(encoders)/sizeof(encoders[0]); ix++) {
    pos[ix] = 0;
    encoders[ix].tick();

    int newPos = encoders[ix].getPosition();
    if (pos[ix] != newPos) {
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

  Serial.println();
  
  delay(1000);
}
