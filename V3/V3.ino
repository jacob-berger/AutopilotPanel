#include <Joystick.h>
#include <BitsAndDroidsFlightConnector.h>
#include <RotaryEncoder.h>
#include "Adafruit_MCP23017.h"

BitsAndDroidsFlightConnector connector(true);
RotaryEncoder spd(2, 3, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder hdg(4, 5, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder alt(6, 7, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder vs(8, 9, RotaryEncoder::LatchMode::TWO03);

Adafruit_MCP23017 mcp;

Joystick_ Joystick;

#define numEncoders 4
#define numTacSwitches 12
uint8_t encButtonPins[] = {10, 11, 12, 13};
uint8_t lastStates[16];
uint8_t currentStates[16];
uint8_t held[16];
RotaryEncoder encoders[] = {spd, hdg, alt, vs};
uint8_t lastTime[16];
uint8_t isAP, isRad;
uint8_t pos[4];

//Will re-order buttons

#define locPin 0
#define appPin 1
#define apPin 2
#define fdPin 3
#define atPin 4
#define swap1Pin 5
#define swap2Pin 6
#define modePin 7

#define spdPin 12
#define hdgPin 13
#define altPin 14
#define vsPin 15

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  while (!Serial);

  mcp.begin();      // use default address 0
  for (int ix = 0; ix < 12; ix++) {
    mcp.pinMode(ix, INPUT);
    mcp.pullUp(ix, HIGH);  // turn on a 100K pullup internally
  }
  
  Joystick.begin();
}

void loop() {

//  BITSANDDROIDSCONNECTOR
  connector.dataHandling();

//    displayMCPs();
    readInputs();
    processInputs();
    processEncoders();
  
  delay(10);
}

void readInputs() {

  //TODO: change hold behavior from time to value incrementing
  //read tac switches
  for (int ix = 0; ix < numTacSwitches; ix++) {
      int pressed = !mcp.digitalRead(ix); //low is pressed
      lastStates[ix] = currentStates[ix];
      if (pressed) {
        if (lastStates[ix] == false) {//button pressed
          lastTime[ix] = millis()%100;
          Serial.println(lastTime[ix]);
        }
        currentStates[ix] = true;
      } else {
        currentStates[ix] = false;
        if (lastStates[ix] == true) {//button released
          uint8_t current = millis()%100;
          Serial.println(current);
          Serial.println(current - lastTime[ix]);
          if ((current - lastTime[ix]) > 1000) {
            held[ix] = true;
          }
        }
      }
  }

  //read encoders
  for (int ix = 0; ix < numEncoders; ix++) {
    int pressed = !digitalRead(encButtonPins[ix]);
    lastStates[numTacSwitches + ix] = currentStates[numTacSwitches + ix];
      if (pressed) {
        if (lastStates[numTacSwitches + ix] == false) {//button pressed
          lastTime[numTacSwitches + ix] = millis();
        }
        currentStates[numTacSwitches + ix] = true;
      } else {
        currentStates[numTacSwitches + ix] = false;
        if (lastStates[numTacSwitches + ix] == true) {//button released
          long current = millis();
//          Serial.println(current - lastTime[numTacSwitches + ix]);
          if ((current - lastTime[numTacSwitches + ix]) > 1000) {
            held[numTacSwitches + ix] = true;
          }
        }
      }
  }
}

void processInputs() {
  uint16_t holdTime = 1200;
  //only send commands for changed states
  for (int ix = 0; ix < sizeof(lastStates)/sizeof(lastStates[0]); ix++) {
    if ((currentStates[ix] == false) && (lastStates[ix] == true)) { //only on release
      switch (ix) {
        case locPin:
//          if (held[ix] == true) {
//            //not yet implemented
//            Serial.println("Held loc");
//            Joystick.pressButton(ix);
//            delay(holdTime);
//            Joystick.releaseButton(ix);
//            held[ix] = false;
//          } else {
//            Serial.println("Pressed loc");
//            Serial.println(connector.sendAPLocHold());
//            Joystick.pressButton(ix);
//            delay(50);
//            Joystick.releaseButton(ix);
//          }
          sendCommands(locPin, held[ix], holdTime, "loc");
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
        case 8:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held 8");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed 8");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 9:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held 9");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed 9");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 10:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held 10");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed 10");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
        case 11:
          if (held[ix] == true) {
            //not yet implemented
            Serial.println("Held 11");
            Joystick.pressButton(ix);
            delay(holdTime);
            Joystick.releaseButton(ix);
            held[ix] = false;
          } else {
            Serial.println("Pressed 11");
            Joystick.pressButton(ix);
            delay(50);
            Joystick.releaseButton(ix);
          }
          break;
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
//          if (held[ix] == true) {
////            not yet implemented
//            Serial.println("Held vs");
//            //TODO
//            //include spadnext association with button
//            Joystick.pressButton(ix);
//            delay(holdTime);
//            Joystick.releaseButton(ix);
//            held[ix] = false;
//          } else {
//            Serial.println("Pressed vs");
//            Serial.println(connector.sendAPVSHold());
//            Joystick.pressButton(ix);
//            delay(50);
//            Joystick.releaseButton(ix);
//          }
          sendCommands(vsPin, held[ix], holdTime, "vs");
          break;
      }
      if (held[ix] == true) {
        held[ix] = false;
      }
    }
  }
}

void sendCommands(uint8_t button, boolean held, uint16_t holdTime, String message) {
  if (held) {
    Serial.print("Held ");
    Serial.println(message);
    Joystick.pressButton(button);
    if (held) {
      delay(holdTime);
    } else {
      delay(50);
    }
    Joystick.releaseButton(button);
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
//        Serial.println(newPos);
//        Serial.println(pos[ix]);
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
//        Serial.println(newPos);
//        Serial.println(pos[ix]);
        lastTime[ix] = millis();
        pos[ix] = newPos;
        }
      } else {
        changed = false;
      }

    }
    timedOut = (millis() - start < 2000);
  } while (!timedOut && changed);

}

void displayMCPs() {
  for (int ix = 0; ix < 16; ix++) {
    if (!mcp.digitalRead(ix)) { //low when pressed
      Serial.println(ix);
    }
  }
  
  delay(100);
}
