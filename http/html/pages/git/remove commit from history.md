## If changes are local only
If it's only on your local PC (or noone checked out your changes):

1) Use:
```bash
git log
```
to find the commit you want to remove. Copy hash (the long sqeuence like: e8348ebe553102018c...).

2) Use:
```bash
git rebase -i your_commit_hash_code_comes_here~
```

Just remove the commit you don't need and save the file.

Interactive git rebase can let you also fix the broken commit - there is no need to remove it.

If you pushed changes to the server or someone already got your changes - never change history - it'd cause **serious** problems for your team.


## If you pushed changes to remote
Once you push to the repo, you really don't want to go about changing history. \
However, if you are **absolutely sure** that nobody has pulled/fetched from the repo since your offending commit, you have 2 options.

1)
If you want to remove the "bad" commit altogether (and every commit that came after that), \
do a `git reset --hard ABC` (assuming ABC is the hash of the "bad" commit's elder sibling — the one you want to see as the new head commit of that branch). \
Then do a `git push --force` (or git push -f).

2)
If you just want to edit that commit, and preserve the commits that came after it, \
do a `git rebase -i ABC~`. \
This will launch your editor, showing the list of your commits, starting with the offending one. \
Change the flag from "pick" to "e", save the file and close the editor. \
Then make the necessary changes to the files, and do a git commit -a --amend, then do git rebase --continue. \
Follow it all up with a `git push -f`.

I want to repeat, these options are only available to you if **nobody** has done a pull or fetch that contains your offending commit. \
If they have, doing these steps will just make matters worse.

