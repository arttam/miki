## In order to run X apps on headless system two options available

Run 
```bash
ssh -X <user>@<host>
```
And caller DISPLAY will be passed to headless system

Or
Install X frame buffer
```bash

sudo apt install xvfb

# Followed by
Xvfb :10 -ac -screen 0 1024x768x24 &

# Then run X required app
DISPLAY=:10 /data/home/art/ZSArt/build/MasterRelease/ZoneSenseServer --version

```
