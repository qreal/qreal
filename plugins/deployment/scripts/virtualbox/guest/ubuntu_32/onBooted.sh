#!/bin/bash

# Throwing event that the system was booted:
sudo VBoxControl guestproperty set OSBooted true >/dev/null

# Then waiting a bit for all listeners to process it;
sleep 5s

# And unsetting this parameter for future sessions.
sudo VBoxControl guestproperty unset OSBooted >/dev/null

# Mouting shared folder, convensions to name it 'share'
sudo mount -t vboxsf share /home/qreal/share
