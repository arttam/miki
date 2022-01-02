## Mount S3 under Linux

Install s3fs
```bash
$ sudo apt install s3fs
```

Populate credentials:
```bash

$ echo <ACCESS_KEY>:<SECRET_KEY> ~/.passwd-s3fs
$ cat ~/.passwd-s3fs
$ chmod 600 ~/.passwd-s3fs
```

Create mount point and mount:
```bash
$  mkdir ~/Archive/s3-drive
$  s3fs chub-t1-001 ~/Archive/s3-drive
$  ll ~/Archive/s3-drive
```
