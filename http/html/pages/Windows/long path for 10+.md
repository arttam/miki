# From Windows 10 onwards you can enable long file path on system level

Source: https://docs.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=registry

To overcome usual 260 characters limitation one can use following Registry entry:
```
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem]
"LongPathsEnabled"=dword:00000001
```