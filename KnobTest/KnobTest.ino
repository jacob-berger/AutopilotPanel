#include <Rotary.h>
#include <HID-Project.h>
#include <HID-Settings.h>
#include <SerialTransfer.h>

Rotary speedRotary = Rotary(2, 3);
Rotary headingRotary = Rotary(5, 6);
Rotary altitudeRotary = Rotary(8, 9);
Rotary verticalRotary = Rotary(11, 12);

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

//loc
//appr
//nav
//??
//??
//??

void setup() {
  Serial.begin(9600);
//  Gamepad.begin();
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  delay(1000);
Serial.print("Hello world\n");
}

void loop() {
  unsigned char result = speedRotary.process();
  int pressed = digitalRead(4);
  if (result == DIR_CW) {
//    int currentSpeed = 
//    Serial.println("CW");
    //Increment by 1 unit
//    Gamepad.press(1);
//    Gamepad.release(1);
  } else if (result == DIR_CCW) {
//    Serial.println("CCW");
//    Gamepad.press(2);
//    Gamepad.release(2);
  } else if (pressed == 0) {
      if (pressed != lastSpeedButton) {
//        Gamepad.press(3);
//        Gamepad.release(3);
//        Serial.println("BUTTON");
    }
  }
  lastSpeedButton = pressed;
//  Gamepad.write();
//  Serial.flush();

//  ArrayToInt converter = {1,1,1,1};
//  int rawData = Serial.read();
//  Serial.print("RECEIVED: ");
//  Serial.print(rawData);
//  delay(1000);
}

union ArrayToInt {
  byte array[4];
  uint16_t integer;
};
