#include <RotaryEncoder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <BitsAndDroidsFlightConnector.h>

#define TFT_CS 10
#define TFT_RST -1
#define TFT_DC 8
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
BitsAndDroidsFlightConnector connector(false);

int displayOn = 1;

long int selectedSpeed, selectedHeading, selectedAltitude, selectedVertical = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.setTimeout(15);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextWrap(false);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.setTextSize(2);
}

long last = millis();
String text[] = {"Line one", "Line two", "Line three", "Longer line", "Last line this one"};
void loop() {
  connector.dataHandling();

//  //about 80ms to fill screen
//  tft.setCursor(0, 0);
//  tft.println("Hello world!");
//  tft.println("This is line two!");
//  tft.println("Another line!");
//  tft.println("Maybe one more...");
//  for (int ix = 0; ix < 5; ix++) {
//    last = millis();
//    tft.setCursor(0,0);
//    tft.println(text[ix]);
//    long difference = millis() - last;
//    tft.println(difference);
//    delay(200);
//  }

  
  tft.setCursor(0, 0);
  tft.print(connector.getActiveCom1());
  
  delay(500);
  tft.fillScreen(ST7735_BLACK);


}
