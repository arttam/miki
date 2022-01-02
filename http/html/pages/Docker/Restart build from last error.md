## Debugging build container
```bash
╭─art@pop-os ~/Archive/Kubernetes Course/troubleshooting
╰─➤  docker build -t error .
Sending build context to Docker daemon  2.048kB
Step 1/3 : FROM ubuntu:20.10
 ---> e508bd6d694e
Step 2/3 : RUN mkdir /temp
 ---> Running in cad4d1cbe3fa
Removing intermediate container cad4d1cbe3fa
 ---> a0f15ff4241d
Step 3/3 : RUN apt -y install figlet
 ---> Running in 952967908c5b

WARNING: apt does not have a stable CLI interface. Use with caution in scripts.

Reading package lists...
Building dependency tree...
Reading state information...
E: Unable to locate package figlet
The command '/bin/sh -c apt -y install figlet' returned a non-zero code: 100
```

At this point - last successful layer is one before error (a0f15ff4241d)
So, to continue from that point:
```bash
╭─art@pop-os ~/Archive/Kubernetes Course/troubleshooting
╰─➤  docker run -ti --rm a0f15ff4241d bash                                                                                                                                                                    100 ↵
root@a3344842bfbc:/# apt -y install figlet
Reading package lists... Done
Building dependency tree
Reading state information... Done
E: Unable to locate package figlet
root@a3344842bfbc:/# apt update
...
```
