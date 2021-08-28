#include <BitsAndDroidsFlightConnector.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS        11
#define TFT_RST       9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC        10
#define TFT_MOSI      12
#define TFT_SCLK      13
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
int numLEDs = 16;
int buttonPin = 3;
boolean pressed = false;
int count = 0;
#define inputSize 30
#define apState 0
#define radioState 1
int apAirspeed, apHeading, apAltitude, apVerticalSpeed;
double com1Active, com1Standby, com2Active, com2Standby;

short leds = 0;

//HARD CODED FOR TESTING LEDS
boolean state[] = {1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1}; //Should only need 12


BitsAndDroidsFlightConnector connector(false);

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.setTextSize(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(false);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  delay(100);
  
  Serial.begin(115200);
  Serial.setTimeout(15);

}

void loop() 
{
  connector.dataHandling();
//  advanceWithButton();
  //get variable state from sim
//  getSimVariables();
  updateLEDs();
//  tft.fillScreen(ST77XX_BLACK);
//  tft.setCursor(0, 0);
//  delay(100);
//  long com1 = connector.getActiveCom1();
//  tft.println(com1);
//  tft.println(connector.getActiveCom2());
//  tft.println(connector.getAPFlightDirectorOn());
//  tft.println(connector.getAPMasterOn());
//  tft.println(connector.getAPAvailable());
  delay(1000);
}

void updateLEDs() {
  for (int ix = 4; ix < sizeof(state)/sizeof(state[0]); ix++) {
    //start at loc since spd, hdg, alt, vs have no light
    setLED(ix, state[ix]);
  }
  updateShiftRegister();
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  for (int ix = 0; ix < 16; ix++) {
    if (ix % 4 == 0 && ix != 0) {
      tft.println();
    }
    tft.print(ix);
    tft.print(": ");
    tft.print(state[ix]);
    tft.print(" ");
  }
  tft.println();
  tft.println(com1Active);
  tft.println(com1Standby);
}

void getSimVariables() {
  /*
   * spd        0
   * hdg        1
   * alt        2
   * vs         3
   * loc        4
   * app        5
   * ap         6
   * fd         7
   * at         8
   * swap1      9
   * swap2      10
   * mode       11
   * UNASSIGNED 12
   * UNASSIGNED 13
   * UNASSIGNED 14
   * UNASSIGNED 15
   */
  state[0] = connector.getAPAirspeedHoldOn();
  state[1] = (connector.getAPHeadingLockOn() || connector.getAPNav1LockOn());
  state[2] = connector.getAPAltitudeLockOn();
  state[3] = connector.getAPVerticalHoldOn();
//  state[4] = connector. CURRENTLY NOT AVAILABLE THROUGH CONNECTOR
  state[5] = connector.getAPApproachHoldOn();
  state[6] = connector.getAPMasterOn();
  state[7] = connector.getAPFlightDirectorOn();
  state[8] = connector.getAPAutothrottleOn();
//  state[9] = connector. MAYBE SET LIGHT BASED ON IF RADIO MODE ON
//  state[10] = connector. MAYBE SET LIGHT BASED ON IF RADIO MODE ON
//  state[11] = DIGITAL READ PIN FROM LEONARDO
//  state[12] = connector.
//  state[13] = connector.
//  state[14] = connector.
//  state[15] = connector.

//  apAirspeed CURRENTLY NOT AVAILABLE THROUGH CONNECTOR
  apHeading = connector.getApHeadingLock();
  apAltitude = connector.getApAltLock();
  apVerticalSpeed = connector.getApVerticalSpeed();
  com1Active = connector.getActiveCom1();
  com1Standby = connector.getStandbyCom1();
  com2Active = connector.getActiveCom2();
  com2Standby = connector.getStandbyCom2();
}

void advanceWithButton() {
  if (!digitalRead(3)) {
      setLED(count, true);
      for (int ix = 0; ix < 16; ix++) {
        //clear all other leds
        if (ix != count) {
          setLED(ix, false);
        }
      }
      count++;
      if (count > 15) count = 0;
      Serial.println(count);
      Serial.println(leds, BIN);
      delay(10);
    }
}

void setLED(int led, boolean value) { // 0 indexed
    if (value == true) {
      bitSet(leds, led);
    } else {
      bitClear(leds, led);
    }
    //REMEMBER TO CALL UPDATESHIFTREGISTER OUTSIDE OF FUNCTION AFTER MODIFICATION
//  updateShiftRegister();
}

void updateShiftRegister()
{
   
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   shiftOut(dataPin, clockPin, LSBFIRST, (leds >> 8));
   digitalWrite(latchPin, HIGH);
}
