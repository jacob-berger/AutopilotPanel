#ifndef MY_BUTTON_H
#define MY_BUTTON_H
#include <Arduino.h>
#include <Adafruit_MCP3008.h>
class Button {
  
  private:
    byte channel;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;
    Adafruit_MCP3008 adc;
    byte threshold;
    
  public:
    Button(Adafruit_MCP3008 adc, int channel);
    void init();
    void update();
    byte getState();
    bool isPressed();
};
#endif
