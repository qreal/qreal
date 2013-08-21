#!/usr/bin/tclsh
# Line cannot be too long

set maxLength 120

foreach fileName [getSourceFileNames] {
    set lineNumber 1
    foreach line [getAllLines $fileName] {
        if {[string length $line] > $maxLength} {
            report $fileName $lineNumber "line is longer than $maxLength characters"
        }
        incr lineNumber
    }
}
