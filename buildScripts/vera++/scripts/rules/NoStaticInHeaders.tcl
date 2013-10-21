#! /usr/bin/tclsh

proc checkStatic { fileName } {
  if {![regexp -nocase {.*.h$} $fileName] } { return }
  set lineCount 1
  foreach line [getAllLines $fileName] {
    if {[regexp {^[ \t]*static [ a-zA-Z0-9_]*;} $line] && ![regexp { const } $line]} {
      report $fileName $lineCount "Static vars in headers are not allowed"
    }
    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  checkStatic $fileName
}