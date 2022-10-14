# Set vi mode on PowerShell

Analogue to `set -o vi`

```
Set-PSReadlineOption -EditMode vi
```

To make it permanent for CurrentUserCurrentHost profile
```
PS C:\Users\arturs.tamanis> $PROFILE | Get-Member -Type NoteProperty

   TypeName: System.String

Name                   MemberType   Definition
----                   ----------   ----------
AllUsersAllHosts       NoteProperty string AllUsersAllHosts=C:\Program Files\PowerShell\7\profile.ps1
AllUsersCurrentHost    NoteProperty string AllUsersCurrentHost=C:\Program Files\PowerShell\7\Microsoft.PowerShell_profile.ps1
CurrentUserAllHosts    NoteProperty string CurrentUserAllHosts=C:\Users\arturs.tamanis\Documents\PowerShell\profile.ps1
CurrentUserCurrentHost NoteProperty string CurrentUserCurrentHost=C:\Users\arturs.tamanis\Documents\PowerShell\Microsoft.PowerShell_profile.ps1

PS C:\Users\arturs.tamanis> Test-Path -Path $PROFILE
False
PS C:\Users\arturs.tamanis> New-Item -ItemType File -Path $PROFILE -Force

    Directory: C:\Users\arturs.tamanis\Documents\PowerShell

Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a---          03/08/2022    15:24              0 Microsoft.PowerShell_profile.ps1

PS C:\Users\arturs.tamanis> Test-Path -Path $PROFILE
True
PS C:\Users\arturs.tamanis> nvim $PROFILE
PS C:\Users\arturs.tamanis> cat $PROFILE
Set-PSReadlineOption -EditMode vi
```
More details on profiles [Here](https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_profiles?view=powershell-7.2)