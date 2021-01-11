## If you need to prevent one particular package from upgrading

You can do something like
```bash
sudo apt-mark hold <package_name>
```

For example:
```bash
╭─art@art-G3-3590 ~/Projects/ec2-controller-builder/build_20.17.0_Release/bin  
╰─➤  sudo apt-mark hold terraform
[sudo] password for art: 
terraform set on hold.

╭─art@art-G3-3590 ~/Projects/ec2-controller-builder/build_20.17.0_Release/bin  
╰─➤  sudo apt update
Hit:1 http://gb.archive.ubuntu.com/ubuntu focal InRelease
...
Fetched 901 kB in 4s (236 kB/s)              
Reading package lists... Done
Building dependency tree       
Reading state information... Done
1 package can be upgraded. Run 'apt list --upgradable' to see it.

╭─art@art-G3-3590 ~/Projects/ec2-controller-builder/build_20.17.0_Release/bin  
╰─➤  sudo apt upgrade
Reading package lists... Done
Building dependency tree       
Reading state information... Done
Calculating upgrade... Done
The following packages have been kept back:
  terraform
0 to upgrade, 0 to newly install, 0 to remove and 1 not to upgrade.
```

### In order to *remove* limitation
```bash
sudo apt-mark unhold <package_name>
```