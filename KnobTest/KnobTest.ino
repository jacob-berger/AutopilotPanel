#include <Rotary.h>
#include <HID-Project.h>
#include <HID-Settings.h>
#include <SerialTransfer.h>

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

//loc
//appr
//nav
//??
//??
//??

long int value = 0;
SerialTransfer myTransfer;

void setup() {
  Serial.begin(9600);
  myTransfer.begin(Serial);
//  Gamepad.begin();
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  delay(1000);
Serial.print("Hello world\n");
}

void loop() {
  if (myTransfer.available()) {
    uint16_t received[16];
    for (uint16_t i = 0; i < myTransfer.bytesRead; i++) {
      received[i] = myTransfer.packet.rxBuff[i];
      
      myTransfer.packet.txBuff[i] = received[i];
    }
    
    value += received[3];
    value << 8;
    value += received[2];
    value << 8;
    value += received[1];
    value << 8;
    value += received[0];

    value += 10;

    union {
      uint16_t returnedVal;
      byte myData[4];
    } data;
    data.returnedVal = value;

    for (int ix = 0; ix < 4; ix++) {
      myTransfer.packet.txBuff[ix] = data.myData[ix];
    }
    
    myTransfer.sendData(myTransfer.bytesRead);
    unsigned char result = speedRotary.process();
    int pressed = digitalRead(4);
    if (result == DIR_CW) {
      //Increment by 1 unit
    } else if (result == DIR_CCW) {
    } else if (pressed == 0) {
        if (pressed != lastSpeedButton) {
      }
    }
    lastSpeedButton = pressed;
  //  Gamepad.write();
  //  Serial.flush();
  
  //  ArrayToInt converter = {1,1,1,1};
  //  int rawData = Serial.read();
  //  Serial.print("RECEIVED: ");
  //  Serial.print(rawData);
    delay(5000);
    for (int ix = 0; ix < 16; ix++) {
      Serial.println(received[ix]);
    }

    Serial.println(value);
  }
}

union ArrayToInt {
  byte array[4];
  uint16_t integer;
};
