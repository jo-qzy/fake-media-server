#!/bin/bash

# Do not use command "set -e", because when grep return empty string,
# the command return value is 1, set -e will trigger then exit.

source env_prepare

IMAGE_NAME="clion_debug_image"
IMAGE_TAG="v0.1"
CONTAINER_NAME="clion_debug"
CHECK_STATUS=`docker images | grep $IMAGE_NAME`
#
#if [[ $CHECK_STATUS = "" ]]; then
#    docker build -t $IMAGE_NAME:$IMAGE_TAG .
#fi

docker run -d -p 2222:2222 -v $PROJECT_DIR:/root/$PROJECT_NAME --name $CONTAINER_NAME $IMAGE_NAME:$IMAGE_TAG
