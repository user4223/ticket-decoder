#!/bin/bash

docker build . -t builder -f etc/Dockerfile
docker run -it builder
