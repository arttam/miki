## If id_rsa.pub has passphrase

In order to automate passphrase entry one can do:
```bash
╭─art@pop-os ~/Projects/ZoneSense/zonesense  ‹BETA*›
╰─➤  ssh-agent /usr/bin/zsh

╭─art@pop-os ~/Projects/packer  ‹master*›
╰─➤  ssh-add                                                                                                                                                              
Enter passphrase for /home/art/.ssh/id_rsa:
Identity added: /home/art/.ssh/id_rsa (art@art-G3-3590)

╭─art@pop-os ~/Projects/packer  ‹master*›
╰─➤  ssh-add -l
3072 SHA256:<SHA256 HERE> art@art-G3-3590 (RSA)

╭─art@pop-os ~/Projects/packer  ‹master*›
╰─➤  git pull
Already up-to-date.
```

### Some additinal tips (not tested yet)
Once you have started the SSH agent with:

```bash
eval $(ssh-agent)
```
Do either:

To add your private key to it:
```bash
 ssh-add
```
This will ask you your passphrase just once, and then you should be allowed to push, provided that you uploaded the public key to Github.

To add and save your key permanently on **macOS**:
```bash
 ssh-add -K  
```
This will persist it after you close and re-open it by storing it in user's keychain.

To add and save your key permanently on **Ubuntu** (or equivalent):
```bash
  ssh-add ~/.ssh/id_rsa
```

