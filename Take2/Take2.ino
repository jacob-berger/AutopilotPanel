#include <RotaryEncoder.h>
#include <SoftTimers.h>
#include <HID-Project.h>
#include <HID-Settings.h>
#include <SerialTransfer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Joystick.h>
#include <BitsAndDroidsFlightConnector.h>

#define TFT_CS A0
#define TFT_RST -1
#define TFT_DC A1
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

RotaryEncoder speedRotary(2, 3);
RotaryEncoder headingRotary(5, 6);
RotaryEncoder altitudeRotary(8, 9);
RotaryEncoder verticalRotary(11, 12);

//Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 24, 0, false, false, false, false, false, false, false, false, false, false, false);
Joystick_ Joystick;

int speedButton = 1;
int speedManaged = 0;
int lastSpeedButton = 1;

int headingButton = 1;
int headingManaged = 0;
int lastHeadingButton = 1;

int altitudeButton = 1;
int altitudeManaged = 0;
int lastAltitudeButton = 1;

int verticalButton = 1;
int verticalManaged = 0;
int lastVerticalButton = 1;

int changed = 1;

int lastSpeed = 0;
int lastHeading = 0;
int lastAltitude = 0;
int lastVertical = 0;

int combinedHigh, combinedLow, combinedValue;

int displayOn = 1;

boolean accel = false;
int accelS, accelH, accelA = 0;

int x; 
int pressCount;
String lastRotaryPressed;
boolean lastHeld, held = false;
uint16_t pressed, released; 

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 100;

//loc
//appr
//nav
//??
//??
//??

long int selectedSpeed, selectedHeading, selectedAltitude, selectedVertical, highSpeed, highHeading, highAltitude, highVertical = 0;

SerialTransfer myTransfer;
SoftTimer screenTimer, accelTimer, buttonTimer;

void setup() {

  //Display setup
  Serial.begin(115200);
  Serial.setTimeout(1);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  delay(100);

  tft.fillScreen(ST7735_BLACK);
  tft.setTextWrap(false);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.setTextSize(2);
  delay(100);
  ///////////////////////////////////////////////////////////////////

  screenTimer.setTimeOutTime(5000);
  screenTimer.reset();
  accelTimer.setTimeOutTime(3000);
  accelTimer.reset();
  buttonTimer.setTimeOutTime(2000);
//  buttonTimer.reset();
  pinMode(4, INPUT);
  pinMode(7, INPUT);
  pinMode(10, INPUT);
  pinMode(13, INPUT);
  pinMode(A2, OUTPUT);

  selectedSpeed = 220;
  selectedHeading = 10;
  selectedAltitude = 5000;
  selectedVertical = 0;

  Joystick.begin();

  BitsAndDroidsFlightConnector connector(true);

} //end setup

void printValues() {
  tft.setCursor(0, 0);
  tft.print("Spd:  ");
  tft.print(selectedSpeed);
  tft.print("   \n");
  tft.print("Hdg:  ");
  tft.print(selectedHeading);
  tft.print("   \n");
  tft.print("Alt:  ");
  tft.print(selectedAltitude);
  tft.print("   \n");
  if (selectedVertical < 0) {
    tft.print("V/S: ");
  } else {
    tft.print("V/S:  ");
  }
  tft.print(selectedVertical);
  tft.print("   \n");
}

void loop() {
  if (displayOn == 1) {
    digitalWrite(A2, HIGH);
  } else {
    digitalWrite(A2, LOW);
  }

  if (Serial.available()) {
    //Compare info from SC with current values
    lastSpeed = selectedSpeed;
    lastHeading = selectedHeading;
    lastAltitude = selectedAltitude;
    lastVertical = selectedVertical;
    
    selectedSpeed = Serial.readStringUntil(',').toInt();
    selectedHeading = Serial.readStringUntil(',').toInt();
    selectedAltitude = Serial.readStringUntil(',').toInt();
    selectedVertical = Serial.readStringUntil('\r').toInt();
//
//    if (lastSpeed != selectedSpeed) {
////      alignVars("speed", lastSpeed, selectedSpeed);
//      adjustValue(false, "speed", lastSpeed > selectedSpeed, abs(lastSpeed - selectedSpeed ));
//    } else if (lastHeading != selectedHeading) {
////      alignVars("heading", lastHeading, selectedHeading);
//    } else if (lastAltitude != selectedAltitude) {
////      alignVars("altitude", lastAltitude, selectedAltitude);
//    } else if (lastVertical != selectedVertical) {
////      alignVars("vertical", lastVertical, selectedVertical);
//    }
    
  }
  if (screenTimer.hasTimedOut()) {
    printValues();
    screenTimer.reset();
  }

//  if (buttonTimer.hasTimedOut()) {
//    if (pressCount != 0) {
//      uint16_t difference = pressed - released;
//      Serial.println(pressed);
//      Serial.println(released);
//      Serial.println(difference);
//      Serial.println(pressCount);
//      Serial.println();
//    }
//    pressCount = 0;
//    lastHeld = false;
//    pressed = 0;
//    released = 0;
//    buttonTimer.reset();
//  }

  if (accelTimer.hasTimedOut()) {
    boolean apply = false;
    //Get number of turns since last check
    int newPos = speedRotary.getPosition();
    if (abs(newPos - accelS) > 5) {
      apply = true;
    }
    accelS = newPos;

    newPos = headingRotary.getPosition();
    if (abs(newPos - accelH) > 5) {
      apply = true;
    }
    accelH = newPos;

    newPos = altitudeRotary.getPosition();
    if (abs(newPos - accelA) > 5) {
      apply = true;
    }
    accelA = newPos;

    //get rid of acceleration for now
    if (apply) {
      accel = true;
      apply = false;
    } else {
      accel = false;
    }

    accelTimer.reset();
  }

  static int pos1, pos2, pos3, pos4 = 0;
  speedRotary.tick();
  int newPos = speedRotary.getPosition();
  if (pos1 != newPos) {
    if (pos1 < newPos) {
      if (accel) {
        selectedSpeed+=10;
      } else {
        selectedSpeed++;
      }
      adjustValue(accel, "speed", true); 
    } else {
      if (accel) {
        selectedSpeed-=10;
      } else {
        selectedSpeed--;
      }
      adjustValue(accel, "speed", false);
    }
    pos1 = newPos;
    changed = 1;
  }

  headingRotary.tick();
  newPos = headingRotary.getPosition();
  if (pos2 != newPos) {
    if (pos2 < newPos) {
      if (accel) {
        selectedHeading+=10;
      } else {
        selectedHeading++;
      }
      adjustValue(accel, "heading", true); 
    } else {
      if (accel) {
        selectedHeading-=10;
      } else {
        selectedHeading--;
      }
      adjustValue(accel, "heading", false);
    }
    pos2 = newPos;
    changed = 1;
  }

  altitudeRotary.tick();
  newPos = altitudeRotary.getPosition();
  if (pos3 != newPos) {
    if (pos3 < newPos) {
      if (accel) {
        selectedAltitude+=1000;
      } else {
        selectedAltitude+=100;
      }
      adjustValue(accel, "altitude", true); 
    } else {
      if (accel) {
        selectedAltitude-=1000;
      } else {
        selectedAltitude-=100;
      }
      adjustValue(accel, "altitude", false);
    }
    pos3 = newPos;
    changed = 1;
  }

  verticalRotary.tick();
  newPos = verticalRotary.getPosition();
  if (pos4 != newPos) {
    if (pos4 < newPos) {
      if (accel) {
        selectedVertical+=1000;
      } else {
        selectedVertical+=100;
      }
      adjustValue(accel, "vertical", true); 
    } else {
      if (accel) {
        selectedVertical-=1000;
      } else {
        selectedVertical-=100;
      }
      adjustValue(accel, "vertical", false);
    }
    pos4 = newPos;
    changed = 1;
  }

  if (selectedSpeed < 0) selectedSpeed = 0;
  if (selectedHeading < 1) {
    selectedHeading = 360 - selectedHeading;
  }
  if (selectedHeading == 0) {
    selectedHeading = 360;
  }
  if (selectedHeading > 360) {
    selectedHeading = selectedHeading % 360;
  }

  if (selectedAltitude < 100) selectedAltitude = 100;

  //if speed rotary pressed
  int reading = !digitalRead(4);
//  Serial.println(reading);
  if (reading != lastHeld) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != lastHeld) {
      lastHeld = reading;
      Serial.println("Button pressed");
    }
  }
  
  if (changed) {
    printValues();
    Serial.print("<");
    Serial.print(selectedSpeed);
    Serial.print(",");
    Serial.print(selectedHeading);
    Serial.print(",");
    Serial.print(selectedAltitude);
    Serial.print(",");
    Serial.print(selectedVertical);
    Serial.print(">");
    changed = 0;
  }

}

void adjustValue(boolean accel, String dataType, boolean increasing) {
  int currentValue;
  if (dataType == "speed"){
    currentValue = selectedSpeed;
  } else if (dataType == "heading") {
    currentValue = selectedHeading;
  } else if (dataType == "altitude") {
    currentValue = selectedAltitude;
  } else if (dataType == "vertical") {
    currentValue = selectedVertical;
  }
  adjustValue(accel, dataType, increasing, 1);
}

void adjustValue(boolean accel, String dataType, boolean increasing, int difference) {
  int rotary, top;
//  Serial.println("adjustValue");
  if (dataType == "speed") {
    rotary = 0;
  } else if (dataType == "heading") {
    rotary = 1;
  } else if (dataType == "altitude") {
    rotary = 2;
  } else if (dataType == "vertical") {
    rotary = 3;
  }

  if (accel) {
    top = 10;
  } else {
    top = difference;
  }
  
  for (int ix = 0; ix < top; ix++) {
    if (increasing){
    //Increasing is higher input for button
//    Serial.println("Increasing");
      Joystick.pressButton(2 * rotary + 1);
      delay(50);
      Joystick.releaseButton(2 * rotary + 1);
    } else {
      Joystick.pressButton(2 * rotary);
      delay(50);
      Joystick.releaseButton(2 * rotary);
    }
  }
}

void alignVars(String value, int lastValue, int targetValue) {
//  boolean increasing = false;
//  if ((targetValue - lastValue) > 0) {
//    while (targetValue > lastValue) {
//      adjustValue(false, value, true);
//      lastValue = currentValue;
//    }
//  } else {
//    while (targetValue < lastValue) {
//      adjustValue(false, value, false);
//      lastValue = currentValue;
//    }
//  }
}
