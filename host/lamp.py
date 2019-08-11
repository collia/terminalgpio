#!/usr/bin/env python3
# coding: utf-8
import npyscreen
import curses
import sys
import serial

class App(npyscreen.StandardApp):
    def onStart(self):
        npyscreen.setTheme(npyscreen.Themes.ColorfulTheme)
        self.addForm("MAIN", MainForm, name="Lamp controller")

        
class LogBox(npyscreen.BoxTitle):
    _contained_widget = npyscreen.MultiLineEdit

class HelpBox(npyscreen.BoxTitle):
    _contained_widget = npyscreen.Pager


        
class ModeSelect(npyscreen.BoxTitle):
    _contained_widget = npyscreen.SelectOne
    prev_sent_value = None
    def when_value_edited(self):
        if self.value != self.prev_sent_value:
            self.prev_sent_value = self.value
            self.parent.parentApp.queue_event(npyscreen.Event("mode_value_edited"))

class FreqSelect(npyscreen.BoxTitle):
    _contained_widget = npyscreen.SelectOne
    prev_sent_value = None
    def when_value_edited(self):
        if self.value != self.prev_sent_value:
            self.prev_sent_value = self.value
            self.parent.parentApp.queue_event(npyscreen.Event("freq_value_edited"))

class BoxSliderPercent(npyscreen.BoxTitle):
    _contained_widget = npyscreen.SliderPercent
    prev_sent_value = None
    def when_value_edited(self):
        if self.value != self.prev_sent_value:
            self.prev_sent_value = self.value
            self.parent.parentApp.queue_event(npyscreen.Event("mode_value_edited"))

class BoxSlider(npyscreen.BoxTitle):
    _contained_widget = npyscreen.Slider
    prev_sent_value = None
    def when_value_edited(self):
        if self.value != self.prev_sent_value:
            self.prev_sent_value = self.value
            self.parent.parentApp.queue_event(npyscreen.Event("freq_value_edited"))


class MainForm(npyscreen.FormBaseNew):
    def create(self):
        help_str = ["q - exit",
                    "s - off the lamp",
                    "d - swith to PWM mode",
                    "f - switch on lamp",
                    "5 - set frequency 50 Hz",
                    "6 - set frequency 60 Hz",
                    "0 - set frequency 100 Hz",
                    "2 - set frequency 120 Hz",
                    "alt+Enter - clear terminal window"]

        self.communicator=Communicator(serial_interface)
        
        self.add_event_hander("mode_value_edited", self.mode_value_edited)
        self.add_event_hander("freq_value_edited", self.freq_value_edited)
        
        new_handlers = {
            "6": self.button_config,
            "5": self.button_config,
            "2": self.button_config,
            "0": self.button_config,
            "f": self.button_config,
            "d": self.button_config,
            "s": self.button_config,
            "q": self.exit_func,
            curses.ascii.alt(curses.ascii.NL): self.inputbox_clear
        }
        self.add_handlers(new_handlers)

        y, x = self.useable_space()
        self.ModeSelect = self.add(ModeSelect,  name="Mode",
                                   value=[0,], values=["Off", "PWM", "On"],
                                   scroll_exit=True,
                                   relx = 2,  rely = 2,
                                   max_width=x // 3 - 4, max_height=3*y // 4 - 6 - 6)
        self.frequency = self.add(FreqSelect, name="Freq",
                                  value=[2,], values=["50", "60", "100", "120", "1000", "Other"],
                                  scroll_exit=True,
                                  hidden = True,
                                  relx = x //3 -1, rely = 2,
                                  max_width=x // 3 - 2, max_height=3*y // 4 - 6 - 6)

        self.duty = self.add(BoxSliderPercent, name="Brightness",
                             value=50,
                             hidden = True,
                             relx = 2, rely = 3*y//4 - 6 - 4 ,
                             max_width=2*x // 3 - 5, max_height=3)
        self.extFreq = self.add(BoxSlider, name="Extended freqency",
                                value=100,
                                hidden = True,
                                contained_widget_arguments = {
                                    "out_of":1200,
                                    "lowest": 10,
                                    "step":10},
                                relx = 2, rely =3*y//4 - 3 - 4 ,
                                max_width=2*x // 3 - 5, max_height=3)
        
        self.add(HelpBox, name = "Help", editable=False, relx = 2*x//3-2, rely=2,  max_height=3*y//4-6, max_width=x//3-1, values=help_str)
        
        self.LogBox = self.add(LogBox, footer="Terminal", editable=False, relx = 2, rely = 3*y//4-3, max_height = y//4+1)


        
    def on_ok(self):
        #self.parentApp.setNextForm(None)
        self.exit_func("")

    def build_gpio_command(self, is_on, is_pwm, duty):
        cmd = "gpio a port 0 mode "
        if is_pwm:
            cmd += "pwm {}%\n".format(duty)
        else:
            if is_on:
                cmd += "on\n"
            else:
                cmd += "off\n"
        return cmd
            
    def mode_value_edited(self, event):
        is_on = False
        is_pwm = False
        duty = 50
        
        if 0 in self.ModeSelect.value:
            is_on = False
            is_pwm = False
            self.frequency.hidden = True
            self.duty.hidden = True
            self.extFreq.hidden = True
        elif 1 in self.ModeSelect.value:
            is_on = False
            is_pwm = True
            duty = int(self.duty.value)
            if self.frequency.hidden:
                self.parentApp.queue_event(npyscreen.Event("freq_value_edited"))
            self.frequency.hidden = False
            self.duty.hidden = False
        elif 2 in self.ModeSelect.value:
            is_on = True
            is_pwm = False
            self.frequency.hidden = True
            self.duty.hidden = True
            self.extFreq.hidden = True

        cmd = self.build_gpio_command(is_on, is_pwm, duty)
        self.add_teminal_line(cmd)
        self.communicator.sendCommand(cmd)
        self.frequency.display()
        self.duty.display()
        self.extFreq.display()

    def build_freq_command(self, freq):
        cmd =  "pwm tim 2 freq {}.0\n".format(freq)
        return cmd
            
    def freq_value_edited(self, event):
        freq = 0
        if 0 in self.frequency.value:
            freq = 50
            self.extFreq.hidden = True
        elif 1 in self.frequency.value:
            freq = 60
            self.extFreq.hidden = True
        elif 2 in self.frequency.value:
            freq = 100
            self.extFreq.hidden = True
        elif 3 in self.frequency.value:
            freq = 120
            self.extFreq.hidden = True
        elif 4 in self.frequency.value:
            freq = 1000
            self.extFreq.hidden = True
        else:
            freq = int(self.extFreq.value)
            self.extFreq.hidden = False

        cmd = self.build_freq_command(freq)
        self.add_teminal_line(cmd)
        self.communicator.sendCommand(cmd)
        self.extFreq.display()


    def add_teminal_line(self, line):
        value = self.LogBox.value
        lines = self.LogBox.height - 1
        result = "\n".join(value.split('\n')[-(lines-1):-1])
        result+= '\n'+line
        self.LogBox.value = result
        self.LogBox.display()

    def button_config(self, _input):
        #self.add_teminal_line(str(_input))

        if _input == ord('0'):
            self.frequency.value = 2
            self.frequency.display()
            self.parentApp.queue_event(npyscreen.Event("freq_value_edited"))
        if _input == ord('2'):
            self.frequency.value = 3
            self.frequency.display()
            self.parentApp.queue_event(npyscreen.Event("freq_value_edited"))
        if _input == ord('5'):
            self.frequency.value = 0
            self.frequency.display()
            self.parentApp.queue_event(npyscreen.Event("freq_value_edited"))
        if _input == ord('6'):
            self.frequency.value = 1
            self.frequency.display()
            self.parentApp.queue_event(npyscreen.Event("freq_value_edited"))

        if _input == ord('s'):
            self.ModeSelect.value = 0
            self.ModeSelect.display()
            self.parentApp.queue_event(npyscreen.Event("mode_value_edited"))
        if _input == ord('d'):
            self.ModeSelect.value = 1
            self.ModeSelect.display()
            self.parentApp.queue_event(npyscreen.Event("mode_value_edited"))
        if _input == ord('f'):
            self.ModeSelect.value = 2
            self.ModeSelect.display()
            self.parentApp.queue_event(npyscreen.Event("mode_value_edited"))
        
            

    def inputbox_clear(self, _input):
        self.LogBox.value = ""
        self.LogBox.display()
        
    def exit_func(self, _input):
        self.communicator.close()
        exit(0)

class Communicator:
    def __init__(self, port):
        "Open serial interface"
        self.port = serial.Serial(port=port,
                                  baudrate = 115200)
        self.port.open()
    def sendCommand(self, command):
        self.port.write(command)
    def close(self):
        "close serial"
        self.port.close()

def init_tui(serial):
    serial_interface = serial
    MyApp = App()
    MyApp.run()


serial_interface=""

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Needed one parameter - serial interface")
        exit(1)
    init_tui(sys.argv[1])
