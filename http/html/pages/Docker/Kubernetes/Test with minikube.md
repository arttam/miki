## Testing Kubernetes locally

Get latest [minikube](https://github.com/kubernetes/minikube/releases)
```bash
$ sudo install ~/Downloads/minikube-linux-amd64 /usr/local/bin/minikube
```

Start minikube

!!! \
**It would overwrite ~/.kube/config**
```bash
minikube start
```
Set environment to use running minikube
```bash
eval $(minikube docker-env)
```

Which would create access to _local_ repository

Build contaier **into local repository**
```bash
docker build -t <conatiner-name/tag> .
```

Make sure it's there
```bash
docker images
```

Deploy container into local cluster

Having _deployment.yaml_
```

apiVersion: apps/v1
kind: Deployment
metadata:
  name: pws
spec:
  replicas: 1
  selector:
    matchLabels:
      name: pws
  template:
    metadata:
      labels:
        name: pws
    spec:
      containers:
        - name: pws
          image: local/python-www-server
          ports:
            - containerPort: 8000
          imagePullPolicy: IfNotPresent
---

```

```bash
$ kubeclt apply -f deployment.yaml

```

Make sure all works fine:
```bash
$ kubectl get pods
```

