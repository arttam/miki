### Rofi instead of dmenu

#### i3 configuration
```
bindsym $mod+d exec --no-startup-id "rofi -show drun -run-shell-command '{terminal} -e \\" {cmd}; read -n 1 -s\\"'"
bindsym $mod+Shift+d exec --no-startup-id "rofi -modi drun -show run"
bindsym $mod+Tab exec --no-startup-id "rofi -modi drun -show window"
```

References:
https://wiki.archlinux.org/index.php/Rofi
