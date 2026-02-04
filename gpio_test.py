import pigpio
import time

pi = pigpio.pi()
pi.set_mode(17, pigpio.OUTPUT)
pi.set_mode(27, pigpio.INPUT)

while True:
    pi.write(17, 1)
    print("Read:", pi.read(27))  # Should print 1
    time.sleep(0.5)
    pi.write(17, 0)
    print("Read:", pi.read(27))  # Should print 0
    time.sleep(0.5)
