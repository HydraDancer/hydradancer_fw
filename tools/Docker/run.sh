#!/bin/sh

docker run --mount type=bind,source=.,target=/home/user/mounted -it hydradancer_compile
