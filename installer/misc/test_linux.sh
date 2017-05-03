#!/bin/bash
rsync -a ../trik-studio-offline-linux-gnu64-installer .
#ln -sfv ../trik-studio-offline-linux-gnu64-installer


docker  build -t trikstudio-test-14.04 .
xhost +
docker run  --device=/dev/dri -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix\
  --device=/dev/snd --net=host   --cap-add=ALL -itm 400M \
  --rm trikstudio-test-14.04 /trik-studio-offline-linux-gnu64-installer

