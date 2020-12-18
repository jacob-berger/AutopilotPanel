from SimConnect import *
import serial
import serial.tools.list_ports
from time import sleep
from pySerialTransfer import pySerialTransfer as txfer
import numpy as np

ports = list(serial.tools.list_ports.comports())

port = ''

for p in ports:
    portString = str(p)
    comPort = portString.find('ETH')
    if comPort != -1:
        port = p.name
        
baudrate = 9600

try:
    ard = txfer.SerialTransfer(port, baudrate)
    ard.open()
    sleep(2)
    sm = SimConnect()
    ac = AircraftRequests(sm, _time=2000)

    prevSpeed = 0
    speed = 0
    prevHead = 0
    heading = 0
    prevAlt = 0
    altitude = 0
    prevVS = 0
    vertical = 0

    x = 0
    while 1:
        speed = round(ac.get("AUTOPILOT_AIRSPEED_HOLD_VAR"))
        heading = round(ac.get("AUTOPILOT_HEADING_LOCK_DIR"))
        altitude = round(ac.get("AUTOPILOT_ALTITUDE_LOCK_VAR"))
        vertical = round(ac.get("AUTOPILOT_VERTICAL_HOLD_VAR"))
##        speed = 400
##        heading = 50
##        altitude = 450
##        vertical = 5

        print("Speed: ")
        print(speed)
        print("Heading: ")
        print(heading)
        print("Altitude: ")
        print(altitude)
        print("V/S: ")
        print(vertical)
        print()

        if speed > 256:
            speedHigh = (speed >> 8) & 0xff
            speedLow = speed & 0xff
        else:
            speedHigh = 0
            speedLow = speed

        if heading > 256:
            headingHigh = (heading >> 8) & 0xff
            headingLow = heading & 0xff
        else:
            headingHigh = 0
            headingLow = heading

        if altitude > 256:
            altitudeHigh = (altitude >> 8) & 0xff
            altitudeLow = altitude & 0xff
        else:
            altitudeHigh = 0
            altitudeLow = altitude

        if (vertical > 256) or (vertical < -255):
            verticalHigh = (vertical >> 8) & 0xff
            verticalLow = vertical & 0xff
        else:
            verticalHigh = 0
            verticalLow = vertical
        
        simData = [speedHigh, speedLow, headingHigh, headingLow, altitudeHigh, altitudeLow, verticalHigh, verticalLow]

        send_size = 0
        list_size = ard.tx_obj(simData)
        
        send_size += list_size
        ard.send(send_size)
        sleep(1)
        while not ard.available():
            if ard.status < 0:
                if ard.status == txfer.CRC_ERROR:
                    print('ERROR: CRC_ERROR')
                elif ard.status == txfer.PAYLOAD_ERROR:
                    print('ERROR: PAYLOAD_ERROR')
                elif ard.status == txfer.STOP_BYTE_ERROR:
                    print('ERROR: STOP_BYTE_ERROR')
                else:
                    print('ERROR: {}'.format(ard.status))
##        received = ard.rx_obj(obj_type=type(testArray), obj_byte_size = send_size, list_format='i')
##        print("Received: ")
##        print(received)
##        x = x + 1

    ard.close()

except KeyboardInterrupt:
    try:
        ard.close()
    except:
        pass
    
except:
    import traceback
    traceback.print_exc()
    
    try:
        ard.close()
    except:
        pass
