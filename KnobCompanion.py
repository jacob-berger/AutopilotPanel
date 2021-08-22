from SimConnect import *
import serial
import time
from serial.serialutil import Timeout
import serial.tools.list_ports
from time import sleep
from pySerialTransfer import pySerialTransfer as txfer
import numpy as np


def split(toSplit):
    if toSplit > 256 or toSplit < -255:
        splitHigh = (toSplit >> 8) & 0xff
        splitLow = toSplit & 0xff
    else:
        splitHigh = 0
        splitLow = toSplit

    result = [splitHigh, splitLow]

    return result


ports = list(serial.tools.list_ports.comports())

port = ''

for p in ports:
    portString = str(p)
    comPort = portString.find('ETH')
    if comPort != -1:
        port = p.name

baudrate = 115200

arduino = serial.Serial(port, baudrate=115200, timeout=.1)
def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data

try:
    sm = SimConnect()
    ac = AircraftRequests(sm, _time=2000)

    #get initial values from sim
    speed = round(ac.get("AUTOPILOT_AIRSPEED_HOLD_VAR"))
    prevSpeed = speed
    heading = round(ac.get("AUTOPILOT_HEADING_LOCK_DIR"))
    prevHead = heading
    altitude = round(ac.get("AUTOPILOT_ALTITUDE_LOCK_VAR"))
    prevAlt = altitude
    vertical = round(ac.get("AUTOPILOT_VERTICAL_HOLD_VAR"))
    prevVS = vertical

    timeout = 2
    changedLast = "sim"
    lastValid = []
    while 1:

        print(f'Speed: {speed}')
        print(f'Heading: {heading}')
        print(f'Altitude: {altitude}')
        print(f'Vertical: {vertical}')
        print(changedLast)
        print()

        if (timeout < 1):
            print("Timeout")
            print(lastValid)
            if (changedLast == "sim"):
                ac.set("AP_SPD_VAR", prevSpeed)
                ac.set("AP_HDG_VAR", prevHead)
                ac.set("AP_ALT_VAR_SET_ENGLISH", prevAlt)
                ac.set("AP_VS_VAR", prevVS)
            elif (len(lastValid) > 0 and changedLast == "ard"):
                toSync = True
                while toSync:
                    inSpeed = round(ac.get("AUTOPILOT_AIRSPEED_HOLD_VAR"))
                    inHeading = round(ac.get("AUTOPILOT_HEADING_LOCK_DIR"))
                    inAltitude = round(ac.get("AUTOPILOT_ALTITUDE_LOCK_VAR"))
                    inVertical = round(ac.get("AUTOPILOT_VERTICAL_HOLD_VAR"))
                    if (inSpeed < lastValid[0]):
                        ac.set("AP_SPD_VAR", inSpeed + 1)
                    if (inSpeed > lastValid[0]):
                        ac.set("AP_SPD_VAR", inSpeed - 1)
                    if (inHeading < lastValid[1]):
                        ac.set("AP_HDG_VAR", inHeading + 1)
                    if (inHeading > lastValid[1]):
                        ac.set("AP_HDG_VAR", inHeading - 1)
                    if (inAltitude < lastValid[2]):
                        ac.set("AP_ALT_VAR_SET_ENGLISH", inHeading + 1)
                    if (inAltitude > lastValid[2]):
                        ac.set("AP_ALT_VAR_SET_ENGLISH", inHeading - 1)
                    if (inVertical < lastValid[3]):
                        ac.set("AP_VS_VAR", inVertical + 1)
                    if (inVertical > lastValid[3]):
                        ac.set("AP_VS_VAR", inVertical - 1)
                    if (inSpeed == lastValid[0] and inHeading == lastValid[1] and inAltitude == lastValid[2] and inVertical == lastValid[3]):
                        print(ac.get("AUTOPILOT_AIRSPEED_HOLD_VAR"))
                        print(ac.get("AUTOPILOT_HEADING_LOCK_DIR"))
                        print(ac.get("AUTOPILOT_ALTITUDE_LOCK_VAR"))
                        print(ac.get("AUTOPILOT_VERTICAL_HOLD_VAR"))
                        print(lastValid)
                        toSync = False
            

        #check for changes from arduino
        changed = arduino.readline()
        if (changed):
            changedLast = "ard"
            timeout = 2
            #look for < and >
            start = changed.find(b'<')
            end = changed.find(b'>')
            if (start != -1 and end != -1 and start < end):
                #trim
                #split and create array
                raw = changed[1:-1]
                dataIn = raw.split(b',')
                #if length of array != 4 flush buffer
                if (len(dataIn) == 4):
                    speed = int(dataIn[0])
                    heading = int(dataIn[1])
                    altitude = int(dataIn[2])
                    vertical = int(dataIn[3])
                    
                    prevSpeed = speed
                    prevHead = heading
                    prevAlt = altitude
                    prevVS = vertical

                    ac.set("AP_SPD_VAR", speed)
                    ac.set("AP_HDG_VAR", heading)
                    ac.set("AP_ALT_VAR_SET_ENGLISH", altitude)
                    ac.set("AP_VS_VAR", vertical)

                    lastValid = [speed, heading, altitude, vertical]
            else:
                arduino.flushInput()
                arduino.flushOutput()
                sleep(0.1)

        #check for changes from sim and push
        else:
            if((speed is not None) and (heading is not None) and (altitude is not None) and (vertical is not None)):
                #getting current values from sim
                speed = round(ac.get("AUTOPILOT_AIRSPEED_HOLD_VAR"))
                heading = round(ac.get("AUTOPILOT_HEADING_LOCK_DIR"))
                altitude = round(ac.get("AUTOPILOT_ALTITUDE_LOCK_VAR"))
                vertical = round(ac.get("AUTOPILOT_VERTICAL_HOLD_VAR"))

                if (speed != prevSpeed or heading != prevHead or altitude != prevAlt or vertical != prevVS):
                    changedLast = "sim"
                    full = str(speed) + "," + str(heading) + "," + str(altitude) + "," + str(vertical) + "\n"
                    arduino.write(bytes(full, 'utf-8'))
                    prevSpeed = speed
                    prevHead = heading
                    prevAlt = altitude
                    prevVS = vertical
        timeout -= 1
        sleep(1)


except KeyboardInterrupt:
    try:
        arduino.close()
    except:
        pass

# except:
#     import traceback
#     traceback.print_exc()

#     try:
#         ard.close()
#     except:
#         pass
