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

savedPwd=$(pwd)

cd "$(dirname "$0")"

logFile=$savedPwd/checker-log.txt

function log {
	echo $1 >> "$logFile"
}

log "$( date "+%F %T" ): Check started ================================================================================"

reportFile=$savedPwd/report
trajectoryFile=$savedPwd/trajectory
failedFieldFile=$savedPwd/failed-field

internalErrorMessage="[ { \"level\": \"error\", \"message\": \"Внутренняя ошибка системы проверки, обратитесь к разработчикам\" } ]"
solutionFailedOnOwnFieldMessage="[ { \"level\": \"error\", \"message\": \"Решение работает неправильно\" } ]"
solutionFailedOnOtherFieldMessage="[ { \"level\": \"error\", \"message\": \"Решение неправильно работает на одном из тестовых полей\" } ]"

[ "$#" -lt 1 ] && show_help || :

fileWithPath=$savedPwd/$1
fileName="${fileWithPath##*/}"
fileNameWithoutExtension="${fileName%.*}"

if ! [ -f "$fileWithPath" ]; then
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

chmod +x "$twoDModel"
chmod +x "$patcher"

export LD_LIBRARY_PATH=.

rm -rf "$savedPwd/reports/$fileNameWithoutExtension"
rm -rf "$savedPwd/trajectories/$fileNameWithoutExtension"

rm -f "$reportFile"
rm -f "$trajectory"
rm -f "$failedFieldFile"

mkdir -p "$savedPwd/reports/$fileNameWithoutExtension"
mkdir -p "$savedPwd/trajectories/$fileNameWithoutExtension"

if [ ! -f "$savedPwd/fields/$fileNameWithoutExtension/no-check-self" ]; then
	log "Running save with its own field"

	$twoDModel --platform minimal -b "$fileWithPath" \
			--report "$savedPwd/reports/$fileNameWithoutExtension/_$fileNameWithoutExtension" \
			--trajectory "$savedPwd/trajectories/$fileNameWithoutExtension/_$fileNameWithoutExtension"

	exitCode=$?

	cat "$savedPwd/reports/$fileNameWithoutExtension/_$fileNameWithoutExtension" > "$reportFile"
	cat "$savedPwd/trajectories/$fileNameWithoutExtension/_$fileNameWithoutExtension" > "$trajectoryFile"

	if [ $exitCode -ne 0 ]; then
		log "Solution failed on its own field, aborting"
		echo $solutionFailedOnOwnFieldMessage
		cat "$reportFile"
		exit 1
	fi
fi

log "Looking for prepared testing fields..."

if [ -d "$savedPwd/fields/$fileNameWithoutExtension" ]; then
	log "Found $savedPwd/fields/$fileNameWithoutExtension folder"

	solutionCopy=$fileNameWithoutExtension-copy.qrs
	cp -f $fileWithPath ./$solutionCopy

	for i in $( ls "$savedPwd/fields/$fileNameWithoutExtension" ); do
		if [ "$i" == "no-check-self" ]; then
			continue
		fi

		log "Field: $i, running $patcher $solutionCopy $savedPwd/fields/$fileNameWithoutExtension/$i..."
		$patcher "$solutionCopy" "$savedPwd/fields/$fileNameWithoutExtension/$i"
		if [ $? -ne 0 ]; then
			echo $internalErrorMessage
			log "Patching failed, aborting"
			exit 2
		fi

		log "Running Checker"
		currentField="${i%.*}"
		$twoDModel --platform minimal -b "./$solutionCopy" \
				--report "$savedPwd/reports/$fileNameWithoutExtension/$currentField" \
				--trajectory "$savedPwd/trajectories/$fileNameWithoutExtension/$currentField"

		exitCode=$?
		if [ ! -f $reportFile ]; then
			cat "$savedPwd/reports/$fileNameWithoutExtension/$currentField" > "$reportFile"
			cat "$savedPwd/trajectories/$fileNameWithoutExtension/$currentField" > "$trajectoryFile"
		fi

		if [ $exitCode -ne 0 ]; then
			echo $solutionFailedOnOtherFieldMessage
			log "Test $currentField failed, aborting"
			cat "$savedPwd/reports/$fileNameWithoutExtension/$currentField" > "$reportFile"
			cat "$savedPwd/trajectories/$fileNameWithoutExtension/$currentField" > "$trajectoryFile"
			echo "$savedPwd/fields/$fileNameWithoutExtension/$i" > "$failedFieldFile"
			cat "$reportFile"
			rm -f "$solutionCopy"
			exit 1
		fi

		log "Checker is done"
	done

	rm -f "$solutionCopy"
else
	log "No testing fields found"
fi

cat "$reportFile"
exit 0
