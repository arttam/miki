# Create snapshot of running  Docker instance

## Start victim
```bash
╭─art@uk-lon01-m15-R3 ~/Trials/nginx/hello-art
╰─➤  docker run --name jjj --rm -p 8080:80 nginx
```

## Make desired changes
 ```
╭─art@uk-lon01-m15-R3 ~/Trials/nginx/hello-art
╰─➤  docker exec -ti jjj bash
root@c5fe4b1fbbf2:/# 
...
```

## At desired point - create snapshot
```
╭─art@uk-lon01-m15-R3 ~/Trials/nginx/hello-art
╰─➤  docker commit jjj nginx-builder
sha256:cdaf770c6d3748465c4db08162c441dc0b9f920e2933da09b9e835443576fc4a
```

## Make sure it there
```
╭─art@uk-lon01-m15-R3 ~/Trials/nginx/hello-art
╰─➤  docker images
REPOSITORY                   TAG                               IMAGE ID       CREATED          SIZE
nginx-builder                latest                            cdaf770c6d37   7 seconds ago    342MB
```

