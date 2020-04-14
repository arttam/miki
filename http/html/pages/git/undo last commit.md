### Undo a commit and redo
```
$ git commit -m "Something terribly misguided"             # (1)
$ git reset HEAD~                                          # (2)
<< edit files as necessary >>                              # (3)
$ git add ...                                              # (4)
$ git commit -c ORIG_HEAD                                  # (5)
```
1. This is what you want to undo.
1. This does nothing to your working tree (the state of your files on disk), but undoes the commit and leaves the changes you committed unstaged (so they'll appear as "Changes not staged for commit" in git status, so you'll need to add them again before committing). If you only want to add more changes to the previous commit, or change the commit message1, you could use git reset --soft HEAD~ instead, which is like git reset HEAD~2 but leaves your existing changes staged.
1. Make corrections to working tree files.
1. git add anything that you want to include in your new commit.
1. Commit the changes, reusing the old commit message. reset copied the old head to .git/ORIG_HEAD; commit with -c ORIG_HEAD will open an editor, which initially contains the log message from the old commit and allows you to edit it. If you do not need to edit the message, you could use the -C option.

Beware however that if you have added any new changes to the index, using commit --amend will add them to your previous commit.