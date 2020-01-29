## Git support in VIM, using fugitive plugin

In short - all git command prepended by :G prefix

Like:
```
- Gwrite ==> git add
- Gcommit -m 'Some commit message' ==> git commit
- Gpush ==> git push
- Gstatus ==> git status
- Glog ==> git log 
```

Usefull ones:
```
:G[v]split ==> split current and HEAD versions
:Gdiffsplit ==> split in diff mode
```
