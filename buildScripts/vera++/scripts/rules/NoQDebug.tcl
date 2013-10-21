#! /usr/bin/tclsh

proc findQDebug { fileName } {
  set LineCount 1
  foreach line [getAllLines $fileName] {
    if {[regexp {[ \t]*qDebug.*} $line]} {
      report $fileName $lineCount "QDebug is not allowed"
    }
    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  findQDebug $fileName
}