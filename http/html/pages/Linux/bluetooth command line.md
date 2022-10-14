# Bluetooth via command line:
```bash
╭─art@uk-lon01-m15-R3 ~/Projects/services/sharing/deployment/art-tests  ‹artemis-test›
╰─➤  bluetoothctl
Agent registered
[CHG] Controller B0:A4:60:98:BA:42 Pairable: yes
[bluetooth]# list
Controller B0:A4:60:98:BA:42 uk-lon01-m15-R3 [default]
[bluetooth]# show B0:A4:60:98:BA:42
Controller B0:A4:60:98:BA:42 (public)
	Name: uk-lon01-m15-R3
	Alias: uk-lon01-m15-R3
	Class: 0x000c010c
	Powered: yes
	Discoverable: no
	DiscoverableTimeout: 0x000000b4
	Pairable: yes
	UUID: A/V Remote Control        (0000110e-0000-1000-8000-00805f9b34fb)
	UUID: Audio Source              (0000110a-0000-1000-8000-00805f9b34fb)
	UUID: PnP Information           (00001200-0000-1000-8000-00805f9b34fb)
	UUID: Headset AG                (00001112-0000-1000-8000-00805f9b34fb)
	UUID: Audio Sink                (0000110b-0000-1000-8000-00805f9b34fb)
	UUID: A/V Remote Control Target (0000110c-0000-1000-8000-00805f9b34fb)
	UUID: Generic Access Profile    (00001800-0000-1000-8000-00805f9b34fb)
	UUID: Headset                   (00001108-0000-1000-8000-00805f9b34fb)
	UUID: Generic Attribute Profile (00001801-0000-1000-8000-00805f9b34fb)
	Modalias: usb:v1D6Bp0246d0535
	Discovering: no
Advertising Features:
	ActiveInstances: 0x00
	SupportedInstances: 0x06
	SupportedIncludes: tx-power
	SupportedIncludes: appearance
	SupportedIncludes: local-name
	SupportedSecondaryChannels: 1M
	SupportedSecondaryChannels: 2M
	SupportedSecondaryChannels: Coded
[bluetooth]# select B0:A4:60:98:BA:42
[bluetooth]# power on
Changing power on succeeded
[bluetooth]# agent on
Agent is already registered
[bluetooth]# default-agent
Default agent request successful
[bluetooth]# discoverable on
Changing discoverable on succeeded
[CHG] Controller B0:A4:60:98:BA:42 Discoverable: yes
[bluetooth]# scan on
Discovery started
[CHG] Controller B0:A4:60:98:BA:42 Discovering: yes
[NEW] Device 94:F6:D6:B9:F0:9C Art's Magic Mouse
[NEW] Device F0:65:AE:54:E3:B3 F0-65-AE-54-E3-B3
[NEW] Device 48:AF:CD:05:CE:EE 48-AF-CD-05-CE-EE
[CHG] Device F0:65:AE:54:E3:B3 Name: Gemma's A33
[CHG] Device F0:65:AE:54:E3:B3 Alias: Gemma's A33
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 00001105-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 0000110a-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 0000110c-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 0000110e-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 00001112-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 00001115-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 00001116-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 0000111f-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 0000112d-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 0000112f-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 00001200-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 00001132-0000-1000-8000-00805f9b34fb
[CHG] Device F0:65:AE:54:E3:B3 UUIDs: 3a622e58-c035-4a26-928c-5601dde7c657
[bluetooth]# pair 94:F6:D6:B9:F0:9C
Attempting to pair with 94:F6:D6:B9:F0:9C
[CHG] Device 94:F6:D6:B9:F0:9C Connected: yes
[CHG] Device 94:F6:D6:B9:F0:9C UUIDs: 00001124-0000-1000-8000-00805f9b34fb
[CHG] Device 94:F6:D6:B9:F0:9C UUIDs: 00001200-0000-1000-8000-00805f9b34fb
[CHG] Device 94:F6:D6:B9:F0:9C ServicesResolved: yes
[CHG] Device 94:F6:D6:B9:F0:9C Paired: yes
Pairing successful
[CHG] Device 94:F6:D6:B9:F0:9C ServicesResolved: no
[CHG] Device 94:F6:D6:B9:F0:9C Connected: no
[bluetooth]# trust 94:F6:D6:B9:F0:9C
[CHG] Device 94:F6:D6:B9:F0:9C Trusted: yes
Changing 94:F6:D6:B9:F0:9C trust succeeded
[bluetooth]# connect 94:F6:D6:B9:F0:9C
Attempting to connect to 94:F6:D6:B9:F0:9C
[CHG] Device 94:F6:D6:B9:F0:9C Connected: yes
Connection successful
[CHG] Device 94:F6:D6:B9:F0:9C ServicesResolved: yes
[Art's Magic Mouse]# info
Device 94:F6:D6:B9:F0:9C (public)
	Name: Art's Magic Mouse
	Alias: Art's Magic Mouse
	Class: 0x00002580
	Icon: input-mouse
	Paired: yes
	Trusted: yes
	Blocked: no
	Connected: yes
	LegacyPairing: no
	UUID: Human Interface Device... (00001124-0000-1000-8000-00805f9b34fb)
	UUID: PnP Information           (00001200-0000-1000-8000-00805f9b34fb)
	Modalias: bluetooth:v004Cp0269d0170
	ManufacturerData Key: 0x004c
	ManufacturerData Value:
  03 05                                            ..
	RSSI: -43
	TxPower: 6
[Art's Magic Mouse]# quit
```