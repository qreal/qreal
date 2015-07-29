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

function show_help {
	echo "Usage: check-solution.sh path/to/save/file"
	echo "It will launch checker for all fields found in 'fields/<save file base name>' folder, return 0 if on all fields"
	echo "a program in a save is working correctly or return 1 if it fails on at least one field."
	echo "Detailed report can be found in 'reports/<save file base name>/<field base name> file."
	echo "Robot trajectory can be found in 'trajectories/<save file base name>/<field base name> file."
	echo "Example: check-solution.sh examples/solutions/alongTheBox.qrs"
	echo "See bin/2D-model --help for detailed information"
	exit 0;
}

logFile=./checker-log.txt

function log {
	echo $1 >> $logFile
}

log "$( date "+%F %T" ): Check started ================================================================================"

reportFile=./report
trajectoryFile=./trajectory

internalErrorMessage="[ { \"level\": \"error\", \"message\": \"Внутренняя ошибка системы проверки, обратитесь к разработчикам\" } ]"
solutionFailedOnOwnFieldMessage="[ { \"level\": \"error\", \"message\": \"Решение работает неправильно\" } ]"
solutionFailedOnOtherFieldMessage="[ { \"level\": \"error\", \"message\": \"Решение неправильно работает на одном из тестовых полей\" } ]"

[ "$#" -lt 1 ] && show_help || :

cd "$(dirname "$0")"

fileWithPath=$1
fileName="${fileWithPath##*/}"
fileNameWithoutExtension="${fileName%.*}"

if ! [ -f $fileWithPath ]; then
	echo $internalErrorMessage
	log "File $fileWithPath does not exist, aborting"
	exit 2
fi

if [ -f ./2D-model ]; then
	twoDModel=./2D-model
	patcher=./patcher
else
	twoDModel=./2D-model-d
	patcher=./patcher-d
fi

chmod +x $twoDModel
chmod +x $patcher

export LD_LIBRARY_PATH=.

log "Running save with its own field"

rm -rf reports/$fileNameWithoutExtension
rm -rf trajectories/$fileNameWithoutExtension

rm $reportFile
rm $trajectory

mkdir -p reports/$fileNameWithoutExtension
mkdir -p trajectories/$fileNameWithoutExtension

$twoDModel --platform minimal -b "$fileWithPath" \
		--report "reports/$fileNameWithoutExtension/$fileNameWithoutExtension" \
		--trajectory "trajectories/$fileNameWithoutExtension/$fileNameWithoutExtension"

cat reports/$fileNameWithoutExtension/$fileNameWithoutExtension > $reportFile
cat trajectories/$fileNameWithoutExtension/$fileNameWithoutExtension > $trajectoryFile

if [ $? -ne 0 ]; then
	log "Solution failed on its own field, aborting"
	echo $solutionFailedOnOwnFieldMessage
	cat $reportFile
	exit 1
fi

log "Looking for prepared testing fields..."

if [ -d fields/$fileNameWithoutExtension ]; then
	log "Found  fields/$fileNameWithoutExtension folder"

	for i in $( ls fields/$fileNameWithoutExtension ); do
		log "Field: $i, running $patcher $fileWithPath fields/$fileNameWithoutExtension/$i..."
		$patcher "$fileWithPath" "fields/$fileNameWithoutExtension/$i"
		if [ $? -ne 0 ]; then
			echo $internalErrorMessage
			log "Patching failed, aborting"
			exit 2
		fi

		log "Running Checker"
		currentField="${i%.*}"
		$twoDModel --platform minimal -b "$fileWithPath" --report "reports/$fileNameWithoutExtension/$currentField" \
				--trajectory "trajectories/$fileNameWithoutExtension/$currentField"

		if [ $? -ne 0 ]; then
			echo ;solutionFailedOnOtherFieldMessage
			log "Test $currentField failed, aborting"
			cat reports/$fileNameWithoutExtension/$currentField > $reportFile
			cat trajectories/$fileNameWithoutExtension/$currentField > $trajectoryFile
			cat $reportFile
			exit 1
		fi

		log "Checker is done"
	done
else
	log "No testing fields found"
fi

cat $reportFile
exit 0
