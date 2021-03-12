## NVIDIA on linux usefull comands

Based on https://kb.brightcomputing.com/knowledge-base/using-nvidia-gpus-in-x-application-on-a-headless-node-via-vnc/

### Is NVIDIA visible
```
# module load shared cuda10.2/toolkit
# nvidia-smi 
Wed Sep 30 18:12:15 2020       
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 450.51.06    Driver Version: 450.51.06    CUDA Version: 11.0     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|                               |                      |               MIG M. |
|===============================+======================+======================|
|   0  Tesla M60           On   | 00000000:00:07.0 Off |                  Off |
| N/A   32C    P8    12W / 150W |      0MiB /  8129MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
                                                                               
+-----------------------------------------------------------------------------+
| Processes:                                                                  |
|  GPU   GI   CI        PID   Type   Process name                  GPU Memory |
|        ID   ID                                                   Usage      |
|=============================================================================|
|  No running processes found                                                 |
+-----------------------------------------------------------------------------+
```

Fetch information about the PCI address of the GPU:

```
# nvidia-xconfig --query-gpu-info
Number of GPUs: 1

GPU #0:
  Name      : Tesla M60
  UUID      : GPU-c621812a-c0e1-dfdd-bc4d-9c65264d6956
  PCI BusID : PCI:0:7:0

  Number of Display Devices: 0
```

Create an /etc/X11/xorg.conf file with the following command:

```
# nvidia-xconfig -a --allow-empty-initial-configuration --busid PCI:0:7:0 --no-connected-monitor
```
Where PCI:0:7:0 is an address of the GPU acquired on the previous step.
