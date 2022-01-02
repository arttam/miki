## Read file line by line

```bash
$ while IFS='' read line; do aws s3 ls ${line}; done < ./Check.txt
```

Or, if more elaborate action needed:\
**bash-scrip-file.sh**
```
#!/bin/bash
file="./Untitled.txt"
while IFS='' read line || [[ -n "$line" ]]; do
   eval ${line}
done <$file
```
