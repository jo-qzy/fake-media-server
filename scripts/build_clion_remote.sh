#!/bin/bash

# Do not use command "set -e", because when grep return empty string,
# the command return value is 1, set -e will trigger then exit.

source env_prepare

DOCKERFILE="Dockerfile.debian"
IMAGE_NAME="clion/remote-cpp-env"
IMAGE_TAG="v0.1"
CONTAINER_NAME="clion_debug"
CHECK_STATUS=`docker images | grep $IMAGE_NAME | grep $IMAGE_TAG`

if [[ $CHECK_STATUS = "" ]]; then
    docker build -t $IMAGE_NAME:$IMAGE_TAG -f $DOCKERFILE .
fi

docker run -d --cap-add sys_ptrace -p127.0.0.1:2222:22 --name $CONTAINER_NAME $IMAGE_NAME:$IMAGE_TAG
