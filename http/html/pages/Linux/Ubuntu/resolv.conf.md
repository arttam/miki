## Ubuntu resolv.conf 

### There are some gotchas on resolv.conf and DHCP
[Source](https://manpages.ubuntu.com/manpages/bionic/man5/resolved.conf.5.html)

In essence \
The DNS servers contacted are determined from the global settings in \
/etc/systemd/resolved.conf, the per-link static settings in /etc/systemd/network/*.network \
files, the per-link dynamic settings received over DHCP and any DNS server information \
made available by other system services. See resolved.conf(5) and systemd.network(5) for \
details about systemd's own configuration files for DNS servers. To improve compatibility, \
/etc/resolv.conf is read in order to discover configured system DNS servers, but only if \
it is not a symlink to /run/systemd/resolve/stub-resolv.conf or \
/run/systemd/resolve/resolv.conf (see below).
