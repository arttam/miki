# Moving existing branch

### Rename local brach
``` bash
$ git branch -m ONRAMP-136
```

### Move old remote origins to new ones
``` bash
$ git push origin :OBS-new-project-structure ONRAMP-136
```

### Push changes to remote
``` bash
git push origin -u ONRAMP-136
```
