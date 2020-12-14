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

    long int selectedSpeed, selectedHeading, selectedAltitude, selectedVertical = 0;

    for (int ix = 3; ix >= 0; ix--) {
      if (ix != 0) {
        selectedSpeed += received[ix];
        selectedSpeed << 8;
      } else {
        selectedSpeed += received[ix];
      }
    }

    selectedSpeed += 10; //changing value to ensure working

    union {
      uint16_t returnedVal;
      byte myData[4];
    } speedByte;
    speedByte.returnedVal = selectedSpeed;

    for (int ix = 0; ix < 4; ix++) {
      myTransfer.packet.txBuff[ix] = speedByte.myData[ix];
    }

    /////////////////////////////////////////

    for (int ix = 3; ix >= 0; ix--) {
      if (ix != 0) {
        selectedHeading += received[ix + 4];
        selectedHeading << 8;
      } else {
        selectedHeading += received[ix + 4];
      }
    }

    selectedHeading += 10; //changing value to ensure working

    union {
      uint16_t returnedVal;
      byte myData[4];
    } headingByte;
    headingByte.returnedVal = selectedHeading;

    for (int ix = 4; ix < 8; ix++) {
      myTransfer.packet.txBuff[ix] = headingByte.myData[ix - 4];
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
  }
}

union ArrayToInt {
  byte array[4];
  uint16_t integer;
};
