#!/usr/bin/tclsh
# Checks if there is pragma once and there is no ifdef

proc checkGuards { fileName } {
  if {![regexp -nocase {.*.h$} $fileName] } { return }
  set lineCount 1 
  set guardFound 0
  foreach line [getAllLines $fileName] {
    if {[regexp {^#pragma once} $line] } {
      set guardFound 1 
    }
    if {[regexp {^[\t ]*#ifn?def .*} $line] } {
      set nextLine [getLine $fileName [expr $lineCount + 1]]
      if {[regexp {^[\t ]*#define .*} $nextLine]} {
	set guardFound 1
	report $fileName $lineCount "Old-style include guard"
      }
    }
    incr lineCount
  }
  if {![expr $guardFound]} {
    report $fileName 1 "No include guard in header file or there is BOM"
  }
}

foreach fileName [getSourceFileNames] {
  checkGuards $fileName
}
