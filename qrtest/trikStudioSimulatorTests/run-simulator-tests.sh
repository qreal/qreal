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

cd "$(dirname "$0")"

rm -f failed-tests

if ! [ -f ./check-solution.sh ]; then
	echo "No 'check-solution.sh' script in current directory, tests can not run. Check your build configuration."
	exit 1
fi

if ! [ -d solutions ]; then
	echo "No 'solutions' directory found, no tests to run. Check your build configuration."
	exit 1
fi

status=0

for i in $( ls solutions ); do
	echo "Running tests on: $i..."
	(. ./check-solution.sh "solutions/$i")
	if [ $? -ne 0 ]; then
		fileNameWithoutExtension="${i%.*}"
		echo "Test $i failed, failed log is:"
		cat report
		echo $i >> failed-tests
		cat report >> failed-tests
		echo "==================" >> failed-tests
		status=1
	fi
done

if [ $status -ne 0 ]; then
	cat failed-tests
fi

exit $status
