## Delete all lines that in one file, but not another

Nice tool: **comm**
> comm - compare two sorted files line by line

```bash
#find lines only in file1
comm -23 file1 file2 

#find lines only in file2
comm -13 file1 file2 

#find lines common to both files
comm -12 file1 file2 
```

