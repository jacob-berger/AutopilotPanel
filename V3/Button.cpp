#include <Adafruit_MCP3008.h>
class Button {
  private:
    byte channel;
    Adafruit_MCP3008 adc;
    byte state;
    byte lastReading;
    byte threshold = 800;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;
  public:
    Button(Adafruit_MCP3008 adc, int channel) {
      this->channel = channel;
      this->adc = adc;
      lastReading = LOW;
      init();
    }
    void init() {
//      pinMode(channel, INPUT);
      update();
    }
    void update() {
      // You can handle the debounce of the button directly
      // in the class, so you don't have to think about it
      // elsewhere in your code
      byte newReading = adc.readADC(channel);
      
      if (newReading != lastReading) {
        lastDebounceTime = millis();
      }
      if (millis() - lastDebounceTime > debounceDelay) {
        // Update the 'state' attribute only if debounce is checked
        if (newReading > threshold) {
             state = newReading;
             Serial.println(newReading);
             Serial.println(threshold);
        }
      }
      lastReading = newReading;
    }
    byte getState() {
      update();
      return state;
    }
    bool isPressed() {
      return (getState() == HIGH);
    }
};
