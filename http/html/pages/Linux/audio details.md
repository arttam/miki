### Find your audio devices

Run following:
```bash
uname -a
sudo fuser -v /dev/snd/*
aplay -lL
amixer -c0 #If needed replace that 0 with the proper card from the aplay -l listing
pacmd list-cards
pacmd list-sources
pacmd list-source-outputs
```
