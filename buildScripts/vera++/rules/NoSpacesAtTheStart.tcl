#!/usr/bin/tclsh
# Checks if lines in the source start with no spaces

proc checkTabs { fileName } {
  set lineCount 1
  foreach line [getAllLines $fileName] {
    if {![regexp -nocase {^[^ ]+.*[\n\r]*|^$} $line] } {
      report $fileName $lineCount "String starts with spaces"
    }
    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  checkTabs $fileName
}