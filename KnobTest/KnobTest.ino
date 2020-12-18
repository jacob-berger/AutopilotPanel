#include <Rotary.h>
#include <HID-Project.h>
#include <HID-Settings.h>
#include <SerialTransfer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS A0
#define TFT_RST -1
#define TFT_DC A1
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

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

int changed = 1;

int apSpeed = 100;
int apHead = 15;
int apAlt = 3000;
int apVS = 0;

int combinedHigh, combinedLow;

//loc
//appr
//nav
//??
//??
//??

SerialTransfer myTransfer;

union combined {
  uint16_t value;
  byte myData[4];
}

void setup() {
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  Serial.println("Initialized");

  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  tft.fillScreen(ST7735_BLACK);
  tft.setTextWrap(false);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.setTextSize(1);
  delay(1000);

  myTransfer.begin(Serial);
//  Gamepad.begin();
  pinMode(4, INPUT_PULLUP);
//  pinMode(7, INPUT_PULLUP);
//  pinMode(10, INPUT_PULLUP);
//  pinMode(13, INPUT_PULLUP);
}

void loop() {
  if (myTransfer.available()) {
    uint16_t received[32];
    for (uint16_t i = 0; i < myTransfer.bytesRead; i++) {
      received[i] = myTransfer.packet.rxBuff[i];
      
      myTransfer.packet.txBuff[i] = received[i];
    }

    long int selectedSpeed, selectedHeading, selectedAltitude, selectedVertical, highSpeed, highHeading, highAltitude, highVertical = 0;

//    selectedSpeed = combine(0, received);
//    selectedSpeed = combine(0, 4, received);
    selectedHeading = combine(8, 12, received);
//    selectedAltitude = combine(16, 20, received);
//    selectedVertical = combine(24, 28, received);
    
//    unsigned char result = speedRotary.process();
//    int pressed = digitalRead(4);
//    
//    if (result == DIR_CW) {
//      selectedSpeed++;
//      changed = 1;
//    } else if (result == DIR_CCW) {
//      selectedSpeed--;
//      changed = 1;
//    } else if (pressed == 0) {
//        if (pressed != lastSpeedButton) {
//          
//      }
//    }
//    lastSpeedButton = pressed;

//    if (1) {

      delay(5000);
//      Serial.println(selectedSpeed);
//      Serial.println(selectedHeading);
//      Serial.println(selectedAltitude);
//      Serial.println(selectedVertical);
      for (int ix = 0; ix < 32; ix++) {
        Serial.print(received[ix]);
        Serial.print(",");
      }
      Serial.println();
      Serial.println(selectedSpeed);
      Serial.println(combinedHigh);
      Serial.println(combinedLow);

//      delay(100);
//      tft.setCursor(0, 0);
//      if (changed) tft.fillScreen(ST7735_BLACK);
//      tft.println(selectedSpeed);
//      tft.println(selectedHeading);
//      tft.println(selectedAltitude);
//      tft.println(selectedVertical);
//      changed = 0;
//    }

//2911304277422742
//2912304277422742

  }

}

int combine(int start, int finish, uint16_t received[]) {
  int low, high = 0;

//    union {
//      uint16_t returnedVal;
//      byte myData[4];
//    } speedByte;
    for (int ix = finish; ix > start; ix--) {
    high += received[ix];
    low += received[ix + 4];
  }
    speedByte.returnedVal = selectedSpeed;


//  int combined = high;
//  combined = combined<<8;
//  combined |= low;
//  combinedHigh = high;
//  combinedLow = low;
//  return combined;

//  for (int ix = 3 + start; ix >= start; ix--) {
//    if (ix != 0) {
//      high += received[ix];
//      high << 8;
//    } else {
//      high += received[ix];
//    }
//  }
//  
//  for (int ix = 3 + start; ix >= start; ix--) {
//    if (ix != 0) {
//      selected += received[ix + 4];
//      selected << 8;
//    } else {
//      selected += received[ix + 4];
//    }
  }

//  word result = high * 256 + selected;
//  selected = result;

//    union {
//      uint16_t value;
//      byte myData[4];
//    } combined;
//    combined.value = selected;
//    return combined.value;

//  int combined;
//  combined = high;              //send x_high to rightmost 8 bits
//  combined = combined<<8;         //shift x_high over to leftmost 8 bits
//  combined |= low;

//  return combined;
//}
//    for (int ix = 3; ix >=0; ix--) {
//      if (ix != 0) {
//        highSpeed += received[ix];
//        highSpeed << 8;
//      } else {
//        highSpeed += received[ix];
//      }
//    }
//
//    for (int ix = 3; ix >= 0; ix--) {
//      if (ix != 0) {
//        selectedSpeed += received[ix + 4];
//        selectedSpeed << 8;
//      } else {
//        selectedSpeed += received[ix + 4];
//      }
//    }
//
//    word combined = highSpeed * 256 + selectedSpeed;
//    selectedSpeed = combined;
//
//    union {
//      uint16_t returnedVal;
//      byte myData[4];
//    } speedByte;
//    speedByte.returnedVal = selectedSpeed;
