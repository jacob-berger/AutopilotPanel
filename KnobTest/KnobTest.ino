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

int changed = 0;

int apSpeed = 100;
int apHead = 15;
int apAlt = 3000;
int apVS = 0;

//loc
//appr
//nav
//??
//??
//??

SerialTransfer myTransfer;

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

  Serial.println("Done");
  delay(1000);
//  myTransfer.begin(Serial);
//  Gamepad.begin();
  pinMode(4, INPUT_PULLUP);
//  pinMode(7, INPUT_PULLUP);
//  pinMode(10, INPUT_PULLUP);
//  pinMode(13, INPUT_PULLUP);
}

void loop() {
//  if (myTransfer.available()) {
//    uint16_t received[20];
//    for (uint16_t i = 0; i < myTransfer.bytesRead; i++) {
//      received[i] = myTransfer.packet.rxBuff[i];
//      
//      myTransfer.packet.txBuff[i] = received[i];
//    }
//
//    long int selectedSpeed, selectedHeading, selectedAltitude, selectedVertical = 0;
//
//    long int highSpeed = 0;
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
//    selectedSpeed += 10; //changing value to ensure working
//
//    union {
//      uint16_t returnedVal;
//      byte myData[4];
//    } speedByte;
//    speedByte.returnedVal = selectedSpeed;
//
//    for (int ix = 0; ix < 4; ix++) {
//      myTransfer.packet.txBuff[ix] = speedByte.myData[ix];
//    }
//
//    /////////////////////////////////////////
//
//    for (int ix = 3; ix >= 0; ix--) {
//      if (ix != 0) {
//        selectedHeading += received[ix + 4];
//        selectedHeading << 8;
//      } else {
//        selectedHeading += received[ix + 4];
//      }
//    }
//
//    selectedHeading += 10; //changing value to ensure working
//
//    union {
//      uint16_t returnedVal;
//      byte myData[4];
//    } headingByte;
//    headingByte.returnedVal = selectedHeading;
//
//    for (int ix = 4; ix < 8; ix++) {
//      myTransfer.packet.txBuff[ix] = headingByte.myData[ix - 4];
//    }
//
//    //////////////////////////////////////////
//
//    for (int ix = 3; ix >= 0; ix--) {
//      if (ix != 0) {
//        selectedAltitude += received[ix + 8];
//        selectedAltitude << 8;
//      } else {
//        selectedAltitude += received[ix + 8];
//      }
//    }
//
//    selectedAltitude = selectedAltitude * 100;
//
//    union {
//      uint16_t returnedVal;
//      byte myData[4];
//    } altitudeByte;
//    altitudeByte.returnedVal = selectedAltitude;
//
//    for (int ix = 8; ix < 12; ix++) {
//      myTransfer.packet.txBuff[ix] = altitudeByte.myData[ix - 8];
//    }
//
//    /////////////////////////////////////////////
//
//    for (int ix = 3; ix >= 0; ix--) {
//      if (ix != 0) {
//        selectedVertical += received[ix + 12];
//        selectedVertical << 8;
//      } else {
//        selectedVertical += received[ix + 12];
//      }
//    }
//
//    selectedVertical += 10;
//
//    union {
//      uint16_t returnedVal;
//      byte myData[4];
//    } verticalByte;
//    verticalByte.returnedVal = selectedVertical;
//
//    for (int ix = 12; ix < 16; ix++) {
//      myTransfer.packet.txBuff[ix] = verticalByte.myData[ix - 12];
//    }
//
//    ///////////////////////////////////////////////
//    
//    myTransfer.sendData(myTransfer.bytesRead);
//    
    unsigned char result = speedRotary.process();
    int pressed = digitalRead(4);
    
    if (result == DIR_CW) {
      apSpeed++;
      changed = 1;
    } else if (result == DIR_CCW) {
      apSpeed--;
      changed = 1;
    } else if (pressed == 0) {
        if (pressed != lastSpeedButton) {
          
      }
    }
    lastSpeedButton = pressed;

    if (changed) {
      tft.setCursor(0, 0);
//      tft.fillScreen(ST7735_BLACK);
      tft.print(apSpeed);
      tft.print("                \n");
      tft.println(apHead);
      tft.println(apAlt);
      tft.println(apVS);
      changed = 0;
    }

//    delay(5000);
//    for (int ix = 0; ix < 16; ix++) {
//      Serial.println(received[ix]);
//    }

//    Serial.print("Speed: ");
//    Serial.println(selectedSpeed);
//    Serial.print("Heading: ");
//    Serial.println(selectedHeading);
//    Serial.print("Altitude: ");
//    Serial.println(selectedAltitude);
//    Serial.print("V/S: ");
//    Serial.println(selectedVertical);
//  }

}
