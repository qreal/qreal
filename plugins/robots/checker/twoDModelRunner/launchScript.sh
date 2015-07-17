#!/bin/bash

set -o nounset
set -o errexit

function show_help {
	echo "Usage: checker.sh [--report path/to/report] [--trajectory path/to/trajectory] path/to/save/file"
	echo "Example: checker.sh examples/solutions/alongTheBox.qrs --report /dev/stdout --trajectory trajectory.fifo"
	echo "See bin/2D-model --help for detailed information"
	exit 0;
}

[ "$#" -lt 1 ] && show_help || :

cd "$(dirname "$0")"

chmod +x ./2D-model
LD_LIBRARY_PATH=. && ./2D-model --platform minimal -b "$@"
