## Change docker containers directory

On Ubuntu

if ```/etc/docker/daemon.json``` does not exists - create it


```bash
cat /etc/docker/daemon.json 
{
	"data-root": "/home/art/Archive/Docker/docker-data"
}
```

In order to check:
```bash
sudo docker info
```