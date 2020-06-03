from button import *
import RPi.GPIO as GPIO
import time

P_BUTTON = 17 # adapt to your wiring
P_LED = 15     # adapt to your wiring

def setup():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(P_LED, GPIO.OUT)
    button = Button(P_BUTTON) 
    button.addXButtonListener(onButtonEvent)

def onButtonEvent(button, event):
    global isRunning
    if event == BUTTON_PRESSED:
        print "pressed"
    elif event == BUTTON_RELEASED:
        print "released"
    elif event == BUTTON_LONGPRESSED:
       print "long pressed"
    elif event == BUTTON_CLICKED:
        print "clicked"
    elif event == BUTTON_DOUBLECLICKED:
        print "double clicked"
        isRunning = False
       
setup()
isRunning = True
while isRunning:
    GPIO.output(P_LED, GPIO.HIGH)    
    time.sleep(1)
    GPIO.output(P_LED, GPIO.LOW)    
    time.sleep(1)
GPIO.cleanup()
print "all done"

