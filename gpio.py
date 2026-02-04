from gpiozero import RotaryEncoder
from signal import pause

# Define GPIO pins (BCM numbering)
encoder = RotaryEncoder(a=17, b=27, max_steps=0)  # max_steps=0 means infinite

def clockwise():
    print("Rotated clockwise")

def counter_clockwise():
    print("Rotated counter-clockwise")

encoder.when_rotated_clockwise = clockwise
encoder.when_rotated_counter_clockwise = counter_clockwise

pause()  # Keep the program running
