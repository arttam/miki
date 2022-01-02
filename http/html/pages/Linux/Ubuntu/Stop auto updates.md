## Ubuntu - stop autoupdates

[Source](https://itsfoss.com/auto-updates-ubuntu/)
```bash
art@ip-172-16-40-146:~$ cat /etc/apt/apt.conf.d/20auto-upgrades
APT::Periodic::Update-Package-Lists "1";
APT::Periodic::Unattended-Upgrade "1";
```

