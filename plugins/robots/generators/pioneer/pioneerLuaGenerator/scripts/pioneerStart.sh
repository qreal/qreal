#!/bin/bash

python controller.py --address $1:8000 --runScript

# curl -X POST -I http://127.0.0.1:8080/pioneer/v0.1/start
