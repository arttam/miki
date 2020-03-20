### Change default g++ vesion (downgrade)

#### Downgrading
To run g++-4.5 on an Ubuntu system that defaults to 4.6. As root:
```bash
apt-get install g++-4.5
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.6 100
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.5 50
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.6 100
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.5 50
update-alternatives --install /usr/bin/cpp cpp-bin /usr/bin/cpp-4.6 100
update-alternatives --install /usr/bin/cpp cpp-bin /usr/bin/cpp-4.5 50
update-alternatives --set g++ /usr/bin/g++-4.5
update-alternatives --set gcc /usr/bin/gcc-4.5
update-alternatives --set cpp-bin /usr/bin/cpp-4.5
```

#### Reverting back to default:
```bash
update-alternatives --auto g++
update-alternatives --auto gcc
update-alternatives --auto cpp-bin
```
