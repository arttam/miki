### Screen saver, monitor off settings

X.org has some basic screen saver functionality as well as energy saving features. \
The settings for both can be viewed and changed with the *xset* tool (from the x11-xserver-utils package).
```sh
xset q 
```
displays the current settings in the sections Screen Saver and DPMS (Energy Star).

You can disable the screen saver feature with:
```sh
xset s off
```

The power saving feature can be turned off with
```sh
xset -dpms
```

With these settings the screen should no longer turn off or blank automatically.\
If your main concern is that 5 minutes are to short, you can also just raise the limits for that. 

To enable the screen saver only after 15 minutes (900 seconds) idle time, set the timeout accordingly with
```sh
xset s 900
```

To turn off the monitor after 20 minutes of idling run
```sh
xset dpms 0 0 1200
```

The two 0 values disable standby and suspend respectively, while 1200 sets the timeout for off to 20 minutes. \
(I usually do not use standby or suspend because there seems to be no difference between the three modes on modern TFT-displays.) \
Setting these values also enables DPMS, so you do not need to explicitly run ```xset +dpms``` .
