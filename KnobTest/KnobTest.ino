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
    selectedSpeed = combine(0, received);
    selectedHeading = combine(8, received);
    selectedAltitude = combine(16, received);
    selectedVertical = combine(24, received);
    
    myTransfer.sendData(myTransfer.bytesRead);
    
    unsigned char result = speedRotary.process();
    int pressed = digitalRead(4);
    
    if (result == DIR_CW) {
      selectedSpeed++;
      changed = 1;
    } else if (result == DIR_CCW) {
      selectedSpeed--;
      changed = 1;
    } else if (pressed == 0) {
        if (pressed != lastSpeedButton) {
          
      }
    }
    lastSpeedButton = pressed;

    if (1) {
      tft.setCursor(0, 0);
      if (changed) tft.fillScreen(ST7735_BLACK);
      tft.print(selectedSpeed);
      tft.print("  ");
      tft.println();
      tft.print(selectedHeading);
      tft.print("  ");
      tft.println();
      tft.print(selectedAltitude);
      tft.print("  ");
      tft.println();
      tft.print(selectedVertical);
      tft.print("  ");
      tft.println();
      changed = 0;
//      for (int ix = 0; ix < 8; ix++) {
//        tft.print(received[ix]);
//        tft.print("   \n");
//      }
//      tft.println(selectedHeading);
//      tft.println(selectedAltitude);
//      tft.println(selectedVertical);
//      tft.print(selectedHeading);
//      tft.print("   \n");
    }

  }

}

int combine(int start, uint16_t received[]) {
  int selected, high = 0;

  for (int ix = 3 + start; ix >= start; ix--) {
    if (ix != 0) {
      high += received[ix];
      high << 8;
    } else {
      high += received[ix];
    }
  }
  
  for (int ix = 3 + start; ix >= start; ix--) {
    if (ix != 0) {
      selected += received[ix + 4];
      selected << 8;
    } else {
      selected += received[ix + 4];
    }
  }

    word result = high * 256 + selected;
    selected = result;

    union {
      uint16_t value;
      byte myData[4];
    } combined;
    combined.value = selected;
    return combined.value;
}
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
