## Having docker file like this:

```
ARG AWS_DEFAULT_REGION
ENV AWS_DEFAULT_REGION=$AWS_DEFAULT_REGION

ARG AWS_ACCESS_KEY_ID
ENV AWS_ACCESS_KEY_ID=$AWS_ACCESS_KEY_ID

ARG AWS_SECRET_ACCESS_KEY
ENV AWS_SECRET_ACCESS_KEY=$AWS_SECRET_ACCESS_KEY

RUN aws s3 cp s3://chub-apps-eu-west-1/libs/linux/ubuntu-20.04/MPGSDK-LX64_9.11_2015-03-27.tar.bz2 ./; \
    tar xjf MPGSDK-LX64_9.11_2015-03-27.tar.bz2; \
    rm MPGSDK-LX64_9.11_2015-03-27.tar.bz2
```

One can run build as follows:
```bash
$ docker build -t mirriad/zonesense-builder:ubuntu-9.3 -f Dockerfile-9.3.bamboo \
  --build-arg AWS_SECRET_ACCESS_KEY=<SECRET_KEY> \
  --build-arg AWS_ACCESS_KEY_ID=<ACCESS_KEY> \
  --build-arg AWS_DEFAULT_REGION=<REGION> \
  .
```
