### chroot (recovery)

In case of failure - boot from live usb ("Try Ubuntu without installing")
Then:
```bash
    2  mkdir /tmp/u
    3  sudo mount /dev/<root_partition_of_ubuntu> /tmp/u
    4  sudo mount --bind /dev /tmp/u/dev
    5  sudo mount --bind /sys /tmp/u/sys
    6  sudo mount --bind /proc /tmp/u/pros
    9  sudo mount --bind /proc /tmp/u/proc
   13  sudo mount /dev/<efi_boot_partition> /tmp/u/boot/efi
   14  sudo chroot /tmp/u
```