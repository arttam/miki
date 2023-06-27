# Deleted volume and persistence claim stuck in terminating state

This happens when persistent volume is protected. You should be able to cross verify this:

Command:

```bash
kubectl describe pvc PVC_NAME | grep Finalizers
```

Output:
```bash
Finalizers:    [kubernetes.io/pvc-protection]
```

You can fix this by setting finalizers to null using kubectl patch:

```bash
kubectl patch pvc PVC_NAME -p '{"metadata":{"finalizers": []}}' --type=merge
```

