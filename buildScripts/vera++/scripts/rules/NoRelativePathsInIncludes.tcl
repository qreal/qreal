#! /usr/bin/tclsh

proc checkRelativePathsInIncludes { fileName } {
  set lineCount 1
  foreach line [getAllLines $fileName] {
    if {[regexp {^[ \t]*#include .*\.\..*} $line]} {
      report $fileName $lineCount "Relative paths in includes are not allowed"
    }
    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  checkRelativePathsInIncludes $fileName
}