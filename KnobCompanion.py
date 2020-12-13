from SimConnect import *
import serial
import serial.tools.list_ports
from time import sleep
from pySerialTransfer import pySerialTransfer as txfer

ports = list(serial.tools.list_ports.comports())

port = ''

for p in ports:
    portString = str(p)
    comPort = portString.find('ETH')
    if comPort != -1:
        port = p.name
        
baudrate = 9600

ard = serial.Serial(port, timeout=5)
sm = SimConnect()
ac = AircraftRequests(sm, _time=2000)

x = 0
while x < 5:
    speed = round(ac.get("AUTOPILOT_AIRSPEED_HOLD_VAR"))
    heading = round(ac.get("AUTOPILOT_HEADING_LOCK_DIR"))
    altitude = round(ac.get("AUTOPILOT_ALTITUDE_LOCK_VAR"))
    vertical = round(ac.get("AUTOPILOT_VERTICAL_HOLD_VAR"))

    print("Speed: ")
    print(speed)
    print("Heading: ")
    print(heading)
    print("Altitude: ")
    print(altitude)
    print("V/S: ")
    print(vertical)
    print()
    array = (speed.to_bytes(16, 'little'), heading.to_bytes(16, 'little'), altitude.to_bytes(16, 'little')
, vertical.to_bytes(16, 'little'))
    print(array)
##    bArray = bytearray(array)
    sleep(1)
    x = x + 1


##end of printing data

    ard.write(array.to_bytes)
    sleep(5)
##
##    returned = ard.read()
##    print(returned)


##print("Set speed is: "),
##print(speed)
##
##x = 0
##while x < 20:
##    rawData = ard.read()
##    data = int.from_bytes(rawData, "little")
##    print(data)
##    if (data == 2):
##        speed = speed + 1
##        print("RIGHT")
##        print("New set airspeed is: "),
##        print(speed)
##    elif (data == 1):
##        speed = speed - 1
##        print("LEFT")
##        print("New set airspeed is: "),
##        print(speed)
##    elif (data == 3):
##        print("BUTTON")
##        print("Airspeed hold should be set")
##
##    ac.set("AUTOPILOT_AIRSPEED_HOLD_VAR", speed)
##    x += 1
ard.close()
