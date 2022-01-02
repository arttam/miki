### Docker installation

```bash
$ sudo apt install docker.io
```

#### In order to get to repository - you need to login to docker.io:
```bash
$ sudo /usr/bin/docker login
```

#### First start might fail
```bash
 sudo systemctl start docker
Job for docker.service failed because the control process exited with error code.
See "systemctl status docker.service" and "journalctl -xe" for details.
╭─art@art-G3-3590 /etc/apt  
╰─➤  sudo systemctl status docker.service                                                                                                                                                                                     1 ↵
● docker.service - Docker Application Container Engine
     Loaded: loaded (/lib/systemd/system/docker.service; disabled; vendor preset: enabled)
     Active: failed (Result: exit-code) since Sun 2020-05-03 23:26:52 BST; 5s ago
TriggeredBy: ● docker.socket
       Docs: https://docs.docker.com
    Process: 13466 ExecStart=/usr/bin/dockerd -H fd:// --containerd=/run/containerd/containerd.sock (code=exited, status=1/FAILURE)
   Main PID: 13466 (code=exited, status=1/FAILURE)

May 03 23:26:52 art-G3-3590 systemd[1]: docker.service: Scheduled restart job, restart counter is at 3.
May 03 23:26:52 art-G3-3590 systemd[1]: Stopped Docker Application Container Engine.
May 03 23:26:52 art-G3-3590 systemd[1]: docker.service: Start request repeated too quickly.
May 03 23:26:52 art-G3-3590 systemd[1]: docker.service: Failed with result 'exit-code'.
May 03 23:26:52 art-G3-3590 systemd[1]: Failed to start Docker Application Container Engine.
```

Then it needs network for docker, which can be achieved by:
```bash
# ip link add name docker0 type bridge
# ip addr add dev docker0 172.16.0.1/16
# systemctl daemon-reload 
# systemctl start docker.service
```

### In order not to type sudo every time, add user to docker group
```bash
sudo usermod -aG docker username

or 

sudo usermod -aG docker ${USER} # for current user
```