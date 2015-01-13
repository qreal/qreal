#!/usr/bin/bash

# Pass --help option for getting useful information

VBOX_INSTALL_PATH=/usr/lib/virtualbox VBOX_SDK_PATH=~/Downloads/VirtualBoxSDK python2 build.py "$@"
