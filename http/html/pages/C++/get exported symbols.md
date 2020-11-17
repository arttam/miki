## Get exported symbols from .so

To get a list of exported symbols from a shared library (a .so) under Linux: 

*_objdump_* (included in GNU binutils):

```bash
$ objdump -T /usr/lib/libid3tag.so.0
00009c15 g    DF .text  0000012e  Base        id3_tag_findframe
00003fac g    DF .text  00000053  Base        id3_ucs4_utf16duplicate
00008288 g    DF .text  000001f2  Base        id3_frame_new
00007b73 g    DF .text  000003c5  Base        id3_compat_fixup
...
```