### Copy preserving parent

#### For file
```bash
cp --parent /etc/systemd/system/mirriad-vpn.service ./

tree .
.
└── etc
...
    └── systemd
        └── system
            └── mirriad-vpn.service

```

#### For folder
```bash
cp --parents -r /etc/openvpn ./

tree .
.
└── etc
...
    ├── openvpn
    │   ├── client
    │   │   ├── client.ovpn
    │   │   └── userpass.txt
    │   ├── server
    │   └── update-resolv-conf
...

```