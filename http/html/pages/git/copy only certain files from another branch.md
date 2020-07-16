## Copy only certain files from antoher branch

```bash
git checkout master               # first get back to master
git checkout experiment -- app.js # then copy the version of app.js 
                                  # from branch "experiment"
```