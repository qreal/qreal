#!/bin/bash

$2 $3controller.py --address $1:8000 --runScript

# curl -X POST -I http://127.0.0.1:8080/pioneer/start
