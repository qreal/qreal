#! /usr/bin/tclsh

proc checkClassNumber { fileName } {
  set lineCount 1
  set classesDeclared 0
  set openedBraces 0
  if {![regexp -nocase {.*.h$} $fileName] } { return }
  foreach line [getAllLines $fileName] {
    if {[regexp -nocase {^[ \t]*class [a-z0-9]*} $line] && $openedBraces == 0} {
      incr classesDeclared 
      if {$classesDeclared > 1} { report $fileName $lineCount "Multiple class definitions in one file" }
    }
    if {[regexp {\{} $line]} { incr openedBraces }
    if {[regexp {\}} $line]} { incr openedBraces -1 }
    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  checkClassNumber $fileName
}