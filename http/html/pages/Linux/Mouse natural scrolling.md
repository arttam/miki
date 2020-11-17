## Add mouses natural scrolling systemwide

### In order to enable natural scrolling add **```Option "NaturalScrolling" "True"```** to pointing devices

For example:

```bash
/usr/share/X11/xorg.conf.d/40-libinput.conf
```

```bash
Section "InputClass"
        Identifier "libinput pointer catchall"
        MatchIsPointer "on"
        MatchDevicePath "/dev/input/event*"
        Driver "libinput"
	Option "NaturalScrolling" "True"
EndSection

Section "InputClass"
        Identifier "libinput touchpad catchall"
        MatchIsTouchpad "on"
        MatchDevicePath "/dev/input/event*"
        Driver "libinput"
	Option "NaturalScrolling" "True"
EndSection
```