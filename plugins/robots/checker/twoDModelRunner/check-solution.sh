#!/bin/bash

set -o nounset
set -o errexit

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

[ "$#" -lt 1 ] && show_help || :

cd "$(dirname "$0")"

fileWithPath=$1
fileName="${fileWithPath##*/}"
fileNameWithoutExtension="${fileName%.*}"

if ! [ -f $fileWithPath ]; then
	echo "File $fileWithPath does not exist, aborting"
	exit 1
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

echo "Looking for prepared testing fields..."

if [ -d fields/$fileNameWithoutExtension ]; then
	echo "Found  fields/$fileNameWithoutExtension folder"
	mkdir -p reports/$fileNameWithoutExtension
	mkdir -p trajectories/$fileNameWithoutExtension
	for i in $( ls fields/$fileNameWithoutExtension ); do
		echo "Field: $i, running $patcher $fileWithPath fields/$fileNameWithoutExtension/$i..."
		$patcher "$fileWithPath" "fields/$fileNameWithoutExtension/$i"
		if [ $? -ne 0 ]; then
			echo "Patching failed, aborting"
			exit 1
		fi

		echo "Running Checker"
		currentField="${i%.*}"
		$twoDModel --platform minimal -b "$fileWithPath" --report "reports/$fileNameWithoutExtension/$currentField" \
				--trajectory "trajectories/$fileNameWithoutExtension/$currentField"

		if [ $? -ne 0 ]; then
			echo "Test $currentField failed, aborting"
			exit 1
		fi

		echo "Checker is done"
	done
else
	echo "No testing fields found, running on a field from save file itself"
	mkdir -p reports
	mkdir -p trajectories
	$twoDModel --platform minimal -b "$fileWithPath" --report "reports/$fileNameWithoutExtension" \
			--trajectory "trajectories/$fileNameWithoutExtension"

	exit $?
fi

exit 0
