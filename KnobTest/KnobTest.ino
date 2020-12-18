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

int combinedHigh, combinedLow, combinedValue;

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
  tft.setTextSize(2);
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
    uint16_t received[myTransfer.bytesRead];
    for (uint16_t i = 0; i < myTransfer.bytesRead; i++) {
      received[i] = myTransfer.packet.rxBuff[i];
      
      myTransfer.packet.txBuff[i] = received[i];
    }

    long int selectedSpeed, selectedHeading, selectedAltitude, selectedVertical, highSpeed, highHeading, highAltitude, highVertical = 0;
    delay(1000);
 
    selectedSpeed = combine(0, 3, received);
    selectedHeading = combine(8, 11, received);
    selectedAltitude = combine(16, 19, received);
    selectedVertical = combine(24, 27, received);
    
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

    while (1) {

      delay(100);
      tft.setCursor(0, 0);
      if (changed) tft.fillScreen(ST7735_BLACK);
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
      changed = 0;
    }

  }

}

int combine(int bottom, int top, uint16_t received[]) {
  int low = 0, high = 0;

    for (int ix = top; ix >= bottom; ix--) {
      if (ix != bottom) {
        high += received[ix];
        high << 8;
      } else {
        high += received[ix];
      }
//      Serial.print("High byte is: ");
//      printBinary(high);
//      Serial.println();
    }

    for (int ix = top; ix >= bottom; ix--) {
      if (ix != bottom) {
        low += received[ix + 4];
        low << 8;
      } else {
        low += received[ix + 4];
      }
    }


  uint16_t combined = (high << 8) | low;
//  combined = combined<<8;
//  combined |= low;
  combinedHigh = high;
  combinedLow = low;
  combinedValue = combined;
//  printBinary(combined);
//  Serial.println();
  return combined;
}

void printBinary(byte inByte) {
  for (int ix = 31; ix >= 0; ix--) {
    Serial.print(bitRead(inByte, ix));
  }
}
