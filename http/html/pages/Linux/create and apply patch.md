## Create and apply patch

### Check diff

```bash
diff -u working/slang.c latest/slang.c
```

The **_-u_** (unified) option tells diff to also list some of the un-modified text lines from before and after each of the changed sections. \
These lines are called context lines. \
They help the patch command locate precisely where a change must be made in the original file.

### Making a Patch file

```bash
diff -u working/slang.c latest/slang.c > slang.patch
```

### Applying patch 
```bash
patch -u working.slang.c -i slang.patch
```

The **_-u_** (unified) option lets patch know that the patch file contains unified context lines. \
In other words, we used the **_-u_** option with diff, so we use the **_-u_** option with patch.

We can instruct patch to make a backup copy of patched files before they are changed by using the **_-b_** (backup) option. \
The **_-i_** (input) option tells patch the name of the patch file to use:

```bash	
patch -u -b working.slang.c -i slang.patch
```
