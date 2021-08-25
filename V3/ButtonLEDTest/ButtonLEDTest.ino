boolean led[4];
boolean last[4];
#include <Adafruit_MCP23X17.h>

long del = 100;
int off = 8;

Adafruit_MCP23X17 mcp;
void setup() {
  pinMode(5, OUTPUT);
  pinMode(4, INPUT);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  Serial.begin(115200);

  if (!mcp.begin_I2C()) {
    Serial.println("Error.");
    while (1);
  }

  mcp.pinMode(4, OUTPUT);
  mcp.pinMode(5, OUTPUT);
  mcp.pinMode(6, OUTPUT);
  mcp.pinMode(7, OUTPUT);
  mcp.pinMode(8, INPUT);
  mcp.pinMode(9, INPUT);
  mcp.pinMode(10, INPUT);
  mcp.pinMode(11, INPUT);
}

void loop() {

  //toggle led based on button press
  for (int ix = 0; ix < 4; ix++) {
    boolean input = mcp.digitalRead(ix + 8);
    if (input && !last[ix]) {//button pressed
      led[ix] = !led[ix];
    } else if (!input && last[ix]) {//button released
      mcp.digitalWrite(off - 1 - ix, led[ix]);
    }
    last[ix] = input;
    Serial.println(led[ix]);

  }


}
