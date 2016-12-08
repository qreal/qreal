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
	echo "It will launch checker for all fields found in 'fields/<save file base name>' folder located in the same folder as the save file."
	echo "If a custom fields folder doesn't exist the default fields are taken from '<trikStudio-checker>/fields/<save file base name>' folder,"
	echo "return 0 if on all fields a program in a save is working correctly or return 1 if it fails on at least one"
	echo "field. Detailed report can be found in 'reports/<save file base name>/<field base name> file."
	echo "Robot trajectory can be found in 'trajectories/<save file base name>/<field base name> file."
	echo "Example: check-solution.sh examples/solutions/alongTheBox.qrs"
	echo "See bin/2D-model --help for detailed information"
	exit 0;
}

binFolder="$(dirname "$0")"
fileFolder=$(dirname "$1")
fieldsFolder="$fileFolder/fields"
inputsFolder="$fileFolder/inputs"
[ ! -d "$fieldsFolder" ] && fieldsFolder=$binFolder/../fields

logFile=$(pwd)/checker-log.txt

function log {
	echo $1 >> "$logFile"
}

log "$( date "+%F %T" ): Check started ================================================================================"

reportFile=$(pwd)/report
trajectoryFile=$(pwd)/trajectory
failedFieldFile=$(pwd)/failed-field

internalErrorMessage="[ { \"level\": \"error\", \"message\": \"Внутренняя ошибка системы проверки, обратитесь к разработчикам\" } ]"
incorrectSaveFileMessage="[ { \"level\": \"error\", \"message\": \"Некорректный или испорченный файл с сохранением\" } ]"
solutionFailedOnOwnFieldMessage="[ { \"level\": \"error\", \"message\": \"Решение работает неправильно\" } ]"
solutionFailedOnOtherFieldMessage="[ { \"level\": \"error\", \"message\": \"Решение неправильно работает на одном из тестовых полей\" } ]"

[ "$#" -lt 1 ] && show_help || :

fileWithPath=$1
fileName="${fileWithPath##*/}"
fileNameWithoutExtension="${fileName%.*}"

if ! [ -f "$fileWithPath" ]; then
	echo $internalErrorMessage
	log "File $fileWithPath does not exist, aborting"
	exit 2
fi

if [ -f $binFolder/2D-model ]; then
	twoDModel=$binFolder/2D-model
	patcher=$binFolder/patcher
else
	twoDModel=$binFolder/2D-model-d
	patcher=$binFolder/patcher-d
fi

export LD_LIBRARY_PATH=$binFolder:$LD_LIBRARY_PATH

rm -rf "$(pwd)/reports/$fileNameWithoutExtension"
rm -rf "$(pwd)/trajectories/$fileNameWithoutExtension"

rm -f "$reportFile"
rm -f "$trajectory"
rm -f "$failedFieldFile"

mkdir -p "$(pwd)/reports/$fileNameWithoutExtension"
mkdir -p "$(pwd)/trajectories/$fileNameWithoutExtension"

if [ ! -f "$fieldsFolder/$fileNameWithoutExtension/no-check-self" ]; then
	log "Running save with its own field"

	$twoDModel --platform minimal -b "$fileWithPath" \
			--report "$(pwd)/reports/$fileNameWithoutExtension/_$fileNameWithoutExtension" \
			--trajectory "$(pwd)/trajectories/$fileNameWithoutExtension/_$fileNameWithoutExtension" \
			--input "$inputsFolder/$fileNameWithoutExtension/check-self.json"

	log "$inputsFolder/$fileNameWithoutExtension/check-self.json"

	exitCode=$?

	if [ $exitCode -eq 2 ]; then
		log "Incorrect or corrupt save file $fileWithPath"
		echo $incorrectSaveFileMessage
		exit 1
	fi

	if [ $exitCode -gt 100 ]; then
		log "Checker internal error, exit code: $exitCode"
		echo $internalErrorMessage
		exit 1
	fi

	cat "$(pwd)/reports/$fileNameWithoutExtension/_$fileNameWithoutExtension" > "$reportFile"
	cat "$(pwd)/trajectories/$fileNameWithoutExtension/_$fileNameWithoutExtension" > "$trajectoryFile"

	if [ $exitCode -ne 0 ]; then
		log "Solution failed on its own field, aborting"
		echo $solutionFailedOnOwnFieldMessage
		sync
		cat "$reportFile"
		exit 1
	fi
fi

log "Looking for prepared testing fields in $fieldsFolder..."

if [ -d "$fieldsFolder/$fileNameWithoutExtension" ]; then
	log "Found $fieldsFolder/$fileNameWithoutExtension folder"

	solutionCopy=$fileNameWithoutExtension-copy.qrs
	cp -f $fileWithPath ./$solutionCopy

	for i in $( ls "$fieldsFolder/$fileNameWithoutExtension" ); do
		if [ "$i" == "no-check-self" ]; then
			continue
		fi

		log "Field: $i, running $patcher $solutionCopy $fieldsFolder/$fileNameWithoutExtension/$i..."
		$patcher "$solutionCopy" "$fieldsFolder/$fileNameWithoutExtension/$i"
		if [ $? -ne 0 ]; then
			echo $internalErrorMessage
			log "Patching failed, aborting"
			exit 2
		fi

		log "Running Checker"
		currentField="${i%.*}"
		$twoDModel --platform minimal -b "./$solutionCopy" \
				--report "$(pwd)/reports/$fileNameWithoutExtension/$currentField" \
				--trajectory "$(pwd)/trajectories/$fileNameWithoutExtension/$currentField" \
				--input "$inputsFolder/$fileNameWithoutExtension/$currentField.json"

		log "$inputsFolder/$fileNameWithoutExtension/$currentField.json"


		exitCode=$?

		if [ $exitCode -gt 100 ]; then
			log "Checker internal error, exit code: $exitCode"
			echo $internalErrorMessage
			exit 1
		fi

		if [ ! -f $reportFile ]; then
			cat "$(pwd)/reports/$fileNameWithoutExtension/$currentField" > "$reportFile"
			cat "$(pwd)/trajectories/$fileNameWithoutExtension/$currentField" > "$trajectoryFile"
		fi

		if [ $exitCode -ne 0 ]; then
			echo $solutionFailedOnOtherFieldMessage
			log "Test $currentField failed, aborting"
			cat "$(pwd)/reports/$fileNameWithoutExtension/$currentField" > "$reportFile"
			cat "$(pwd)/trajectories/$fileNameWithoutExtension/$currentField" > "$trajectoryFile"
			echo "$(pwd)/fields/$fileNameWithoutExtension/$i" > "$failedFieldFile"
			sync
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

sync
cat "$reportFile"
exit 0
