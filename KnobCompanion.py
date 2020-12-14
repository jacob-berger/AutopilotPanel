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

try:
    ##ard = serial.Serial(port, timeout=5)
    ard = txfer.SerialTransfer(port, baudrate)
    ard.open()
    sleep(2)
    ##sm = SimConnect()
    ##ac = AircraftRequests(sm, _time=2000)

    x = 0
    while x < 1:
    ## commented out for testing    
    ##    speed = round(ac.get("AUTOPILOT_AIRSPEED_HOLD_VAR"))
    ##    heading = round(ac.get("AUTOPILOT_HEADING_LOCK_DIR"))
    ##    altitude = round(ac.get("AUTOPILOT_ALTITUDE_LOCK_VAR"))
    ##    vertical = round(ac.get("AUTOPILOT_VERTICAL_HOLD_VAR"))
        speed = 100
        heading = 50
        altitude = 3000
        vertical = 5

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
##        print(array)
    ##    bArray = bytearray(array)
        testArray = [speed, heading, altitude, vertical]
        send_size = 0
        list_size = ard.tx_obj(testArray)
        
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
        received = ard.rx_obj(obj_type=type(testArray), obj_byte_size = send_size, list_format='i')
        print("Received: ")
        print(received)
        x = x + 1

    ard.close()
    ##end of printing
    ##ard.write(array.to_bytes)
    ##sleep(5)
    ##
    ##returned = ard.read()
    ##print(returned)


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
