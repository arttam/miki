## Change console font

[Based on](https://www.maketecheasier.com/change-console-fonts-in-ubuntu/)

Changing the console font is very easy. Here is what you need to do. Open a terminal and type:

```bash

sudo dpkg-reconfigure console-setup
```

You will see this: 

consolesetup-set-encoding  
Press “Enter” to select the default.  

Next, it will ask you for the character set the console should support. Similarly, press “Enter” to select the default.  

consolesetup-set-character-set  
The next screen is the one that is most important to you – changing the font used in the console. There are only a few choices: VGA, Fixed, Terminus, TerminusBold and TerminusBoldVGA. Fixed seems to have the best coverage for international scripts while Terminus is the most pleasing to the eyes. Make your choice with the arrow up/down key and press Enter.  

consolesetup-set-font  
Lastly, you can set the font size used in the console. The default is 16, but you can change it to a bigger or smaller font size.  

consolesetup-font-size-message  
consolesetup-set-font-size
To see the font change in action, you have to open a virtual console (not to be confused with the terminal).

1. Press “Alt + Ctrl + F1” on your keyboard. The virtual console should open full screen. Login with your username and password.  

2. Once you are logged into the console, type:  
```bash
setupcon
```
3. You should see the font change immediately.  

4. To get out of the console, press “Alt + Ctrl + F7” on your keyboard. This will return you to the desktop.  

That’s it.  
