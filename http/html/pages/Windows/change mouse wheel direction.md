### Change mouse scrolling wheel direction in Windows 10

1. **Device Manager**
1. Open **Mice and other pointing devices**
1. Right click **Properties**
1. Property -> **Device instance path** 
1. Copy VID_ id, for example: HID\ **VID_046D&PID_C52B&MI_01&COL01** \7&2EA075F1&0&0000
1. Regedit
1. **Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\HID**
1. Expand **copied** key
1. Expand **available** key
1. Select **Device Parameters** key
1. Double click the **FlipFlopWheel** DWORD and set value from **0** to **1**
1. Restart

