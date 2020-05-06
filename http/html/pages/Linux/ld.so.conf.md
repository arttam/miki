### Lirary pathes in Linux

#### Add Your Path
Create a file called /etc/ld.so.conf.d/myapp.conf:
```bash
# vi /etc/ld.so.conf.d/myapp.conf
```

Add the following path:

```
/usr/local/lib
```
Save and close the file.

#### Activate Your Library Path
You must run the following command to activate path:
```bash
# ldconfig
```

Verify Your New Library Path
```bash
# ldconfig -v | less
```

OR
```bash
# ldconfig -v | grep /usr/local/lib
```

Sample outputs:

```bash
/usr/local/lib:
        libGeoIP.so.1 -> libGeoIP.so.1.4.6
        libGeoIPUpdate.so.0 -> libGeoIPUpdate.so.0.0.0
/usr/lib64/mysql:
        libmysqlclient_r.so.15 -> libmysqlclient_r.so.15.0.0
        libmysqlclient.so.15 -> libmysqlclient.so.15.0.0
/lib:
        libutil.so.1 -> libutil-2.5.so
```

#### How Do I Delete The Library Path?
Simply, delete the file:
```bash
# rm /etc/ld.so.conf.d/myapp.conf
# ldconfig
```
