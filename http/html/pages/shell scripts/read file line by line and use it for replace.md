```bash
while read -r line; 
do sed -i 's,'"$line"',Toolbox/'"$line"',g' $(find src/ -type f -name '*h'); 
done < toolbox.headers
```