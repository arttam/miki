## In WSL (Windows Sybsystem Linux) if you forgot your password
Start command prompt in Administrative mode, then
```bash
C:\WINDOWS\system32>wsl -u root
root@uk-lon01-nb56:/mnt/c/WINDOWS/system32# passwd art
Enter new UNIX password:
Retype new UNIX password:
passwd: password updated successfully
root@uk-lon01-nb56:/mnt/c/WINDOWS/system32# exit
logout

C:\WINDOWS\system32>
```
