```bash
while read -r line; do echo "Included: ${line}"; done < toolbox.headers
```

Or
```bash
cat pip-packages.list|while read l; do echo ${l}; done
```
