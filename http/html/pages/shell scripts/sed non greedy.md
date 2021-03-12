# Make sed expression non-greedy

Having file with lines like:
```
...ba","episodeUUID":"0e1f43cf-79af-4972-821c-489b66ee7313","loca...
```

I need only episode UUID

Then I do following
```
while IFS= read -r line; do echo ${line} | sed 's|\(^.*episodeUUID":"\)\([^"]*\)\(.*$\)|\2|g'; done < check.json
```

I.e inside group I need every symbol *_but_* closing one delimiter (double quote) in my case