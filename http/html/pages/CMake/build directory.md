### Specify build folder

In order to specify build folder without creating build folder, changing to build folder and launching cmake from there\

Following options might be used:
```bash
cmake . -Bbuild
```
**Important**: no space after -B.

Or even specifying both source and build folders\
The -B and -H flags

```bash
cmake -Hpath/to/source -Bpath/to/build
```