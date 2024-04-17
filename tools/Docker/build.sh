#!/bin/sh

docker build -t hydradancer_compile --build-arg UID=$(id -u) .

