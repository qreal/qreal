#!/bin/bash
# Copyright 2015 CyberTech Labs Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -o nounset
set -o errexit

function show_help {
	echo "Usage: checker.sh [--report path/to/report] [--trajectory path/to/trajectory] path/to/save/file"
	echo "Example: checker.sh examples/solutions/alongTheBox.qrs --report /dev/stdout --trajectory trajectory.fifo"
	echo "See bin/2D-model --help for detailed information"
	exit 0;
}


[ "$#" -lt 1 ] && show_help || :

$(dirname "$0")/bin/check-solution.sh "$1"
