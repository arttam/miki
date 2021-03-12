## In order to install X on Ubuntu without bunch of not needed stuff

Try this:
```
sudo apt-get install xserver-xorg-core --no-install-recommends --no-install-suggests
```

In case it's not headless box, and you need to start X, add:
```
sudo apt-get install xinit
```

