import vlc
import time


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



try:
    stationNumber = 0
    while True:
            
        print("Turn")
        if player != None:
            player.stop()
        player = vlc.MediaPlayer(stationUrls[stationNumber])
        player.play()
        
        print(f"Now playing : {stationNames[stationNumber]}")
        time.sleep(2)
        if player.is_playing() == False:
            raise Exception(" Couldnt Start : ")

except Exception as e:
    print(f"Station Unavailable {e}")
except KeyboardInterrupt:
    player.stop()
    break
