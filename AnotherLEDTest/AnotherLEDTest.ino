#define latchPin 5
#define clockPin 6
#define dataPin 4

byte leds = 0;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  leds = 0;
  updateShiftRegister();
  delay(500);
//  bitSet(leds, 0);
//  Serial.println(leds, BIN);
//  updateShiftRegister();
//  delay(500);
  for (int ix = 0; ix < 8; ix++) {
    bitSet(leds, ix);
    updateShiftRegister();
    Serial.println(leds, BIN);
    delay(500);
  }
}

void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}
