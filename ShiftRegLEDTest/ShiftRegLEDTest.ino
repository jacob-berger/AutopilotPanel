/*
Adafruit Arduino - Lesson 4. 8 LEDs and a Shift Register
*/

int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
int numLEDs = 16;
int buttonPin = 3;
boolean pressed = false;
int count = 0;

short leds = 0;
byte bank1 = 0;
byte bank2 = 0;
const int bufferSize = 128;
char inputBuffer[bufferSize];
int bufferPointer = 0;
boolean state[16];

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);

//  state[0] = true;
//  state[2] = true;
//  state[4] = true;
//  state[6] = true;
}

void loop() 
{
//  char inByte;
//  if(Serial.available())
//    {
//  inByte = Serial.read();
//  if (inByte == '\n') {
//    // 'newline' character
//    inputBuffer[bufferPointer++] = '\0';
//    leds = atoi(inputBuffer);
//    
//    Serial.println(inputBuffer);
//    bufferPointer = 0;
//  } else {
//  // not a 'newline' character
//    if (bufferPointer < bufferSize - 1) {// Leave room for a null terminator
//      inputBuffer[bufferPointer++] = inByte;
//    }
//  }
//  updateShiftRegister();
//  setLED(leds, true);
//  delay(50);
//  for (int i = 0; i < 8; i++)
//  {
//    bitSet(leds, i);
//    updateShiftRegister();
//    delay(200);
//    Serial.println(leds);
//  }

    if (!digitalRead(3)) {
      setLED(count, true);
      for (int ix = 0; ix < 16; ix++) {
        //clear all other leds
        if (ix != count) {
          setLED(ix, false);
        }
      }
      count++;
      Serial.println(count);
      Serial.println(leds, BIN);
      if (count > 15) count = 0;
      delay(100);
    }

//  for (int ix = 0; ix < numLEDs; ix++) {
//    setLED(ix, true);
//    setLED(7 - ix, false);
//  }
//  delay(100);
//  for (int ix = 0; ix < numLEDs; ix++) {
//    setLED(ix, false);
//  }
//  setLED(0, true);
//  setLED(1, true);
//  setLED(14, true);
//  setLED(15, true);
}

void setLED(int led, boolean value) {
    if (value == true) {
      bitSet(leds, led);
    } else {
      bitClear(leds, led);
    }
//    updateShiftRegister();
    delay(50);
//    Serial.print(bank1, BIN);
//    Serial.print(" ");
//    Serial.println(bank2, BIN);
  updateShiftRegister();
}

void updateShiftRegister()
{
   
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   shiftOut(dataPin, clockPin, LSBFIRST, (leds >> 8));
   digitalWrite(latchPin, HIGH);
}
