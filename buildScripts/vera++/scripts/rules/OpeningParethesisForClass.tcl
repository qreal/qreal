#!/usr/bin/tclsh
# "{" in a class declaration shall be on its own line

foreach fileName [getSourceFileNames] {
    set lineNumber 1
    foreach line [getAllLines $fileName] {
        if {[regexp -lineanchor {^class} $line] && [regexp {\{} $line]} {
            report $fileName $lineNumber "Class declaration shall contain '\{' on a next string"
        }
        incr lineNumber
    }
}
