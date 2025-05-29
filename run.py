import serial
import subprocess
import time

ser = serial.Serial('/dev/ttyACM0', 9600)

while True:
    try:
        line = float(ser.readline().decode('utf-8').strip())
        print("Line read: ", line)
        if line >= 1.0:
            print("Trigger received! Running C++ executable...")
            subprocess.run(["./philipsHue"], check=True)
    except ValueError:
        print("Received invalid data, skipping...")