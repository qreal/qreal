#!/bin/sh
tclsh buildScripts/vera++/generatePaths.tcl
vera++ --error -p allRules --root buildScripts/vera++ <buildScripts/vera++/params
