import vlc
import time
from gpiozero import RotaryEncoder
from signal import pause
import subprocess
 
encoder = RotaryEncoder(17, 18, max_steps=0)  # swap pins if backwards
STEP = 2
 
last = 0


stationUrls = []
stationNames = []
with open("/home/GastonArch/Downloads/Top2525.m3u") as stations:
    lines = stations.readlines()
    for station in lines:
        if station.startswith("http"):
            station = station.strip()
            stationUrls.append(station)
        else:
            n = station.strip("# EXTINF")
            stationNames.append(n)


player = None

#player = vlc.MediaPlayer('https://uk1.streamingpulse.com/ssl/grollooradio')
last = 0
def rotated():
    current = encoder.steps
    return current
try:
    stationNumber = 0
    while True:
        
        print("Turn")
        if player != None:
            player.stop()
        encoder.when_rotated = rotated
        player = vlc.MediaPlayer(stationUrls[rotated()])
        player.play()
        print(f"Now playing : {stationNames[rotated()]}")
        time.sleep(2)
        if player.is_playing() == False:
            raise Exception(" Couldnt Start : ")

except Exception as e:
    print(f"Station Unavailable {e}")
except KeyboardInterrupt:
    player.stop()
    break

