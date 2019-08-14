#!/usr/bin/env python3

import serial
import time
from _thread import start_new_thread


class Communicator:
    def __init__(self, port):
        "Open serial interface"
        #print("Openning "+port);
        self.port = serial.Serial(port=port,
                                  baudrate = 115200)
        #self.port.open()
        start_new_thread(reader, (self, ) )
    def sendCommand(self, command):
        self.port.write(command.encode("ascii", "ignore"))
        #time.sleep(0.5)
    def close(self):
        "close serial"
        self.port.close()
def reader(comm):
    while(1):
        line = comm.port.readline()
        print(str(line))
        
def init_tui(serial):
    global serial_interface
    serial_interface = serial
    MyApp = App()
    MyApp.run()


serial_interface="/dev/ttyACM0"

if __name__ == '__main__':
    c=Communicator(serial_interface)
    sent = 0
    for i in range(0, 100):
        cmd = "gpio a port 0 mode pwm {}%".format(i)
        sent += len(cmd)
        print("Sent {} bytes".format(sent))
        print(cmd)
        c.sendCommand(cmd)
        time.sleep(1)
    c.close()
    
