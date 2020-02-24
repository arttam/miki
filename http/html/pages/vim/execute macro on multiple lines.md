### Execute macro on multiple lines

Use **normal** in Ex mode, for example:

Execute the macro stored in register a on lines 5 through 10.

```vim
:5,10norm! @a
```

Execute the macro stored in register a on lines 5 through the end of the file.

```vim	
:5,$norm! @a
```
Execute the macro stored in register a on all lines.
	
```vim
:%norm! @a
```
Execute the macro store in register a on all lines matching pattern.

```vim
:g/pattern/norm! @a
```

To execute the macro on visually selected lines, \
press *V* and the *j* or *k* until the desired region is selected. \
Then type :

```vim
norm! @a 
```

and observe the that following input line is shown.

```vim
:'<,'>norm! @a
```