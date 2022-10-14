# How to make a program continue to run after log out from ssh

```bash
╭─art@ip-172-16-0-45 /tmp
╰─➤  ./bg.sh
^Z
[1]  + 2716773 suspended  ./bg.sh
╭─art@ip-172-16-0-45 /tmp
╰─➤  disown %1
disown: warning: job is suspended, use `kill -CONT -2716773' to resume
╭─art@ip-172-16-0-45 /tmp
╰─➤  kill -CONT -2716773               
```