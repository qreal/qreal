#! /usr/bin/tclsh
proc getNextLine { fileName currentLine } {
 if {$currentLine < [getLineCount $fileName]} {
  return [getLine $fileName [expr $currentLine + 1]]
 } else {
  return ""
 }
 
}

proc checkIncludeOrder { fileName } {
  # if fileName.h included
  set correspondingHeaderIncluded 0
  # assert all <...> files included
  set systemConnected 0
  set fileShortName ""
  set allIncluded 0
  set nextLine 0
  # extract short name of a file
  regexp {^[^.]*} [file tail $fileName] fileShortName
  # in header file you don't need include fileName.h
  if {[regexp -nocase {.*.h$} $fileName] } { set correspondingHeaderIncluded 1 }
  
  set lineCount 1
  foreach line [getAllLines $fileName] {
    # check if string contains #include "fileName.h"
    if {[regexp [subst -nocommands -nobackslashes {[ \t]*#include[ \t]*\"$fileShortName.h\"[\t ]*}] $line]} {
      incr correspondingHeaderIncluded
      if {$correspondingHeaderIncluded == 1} { 
      incr lineCount
      set nextLine [getNextLine $fileName $lineCount]

      if {![regexp {^$} $nextLine]} {
	report $fileName $[expr $lineCount + 1] "No new line after include"
      }
	continue 
      } else {
	report $fileName [expr $lineCount + 1] "No new line after include" 
      }
    } 
     
    # check if <...> included
    if {[regexp {[\t ]*#include[\t ]*<[^>]*>[ \t]*$} $line]} {
      if {$correspondingHeaderIncluded == 0} { report $fileName $lineCount "Invalid order of includes" }
      if {$systemConnected == 1} {report $fileName $lineCount "Invalid order of include" }
      incr lineCount
      continue
    }
    
    if {[regexp {[\t ]*#include[\t ]*\"[^\"]*\"[ \t]*$} $line]} {
	if {!$systemConnected} {
	
	  set nextLine [getNextLine $fileName $lineCount]

	  if {![regexp {^$} $nextLine]} {
	    report $fileName $[expr $lineCount + 1] "No new line after include"
	  }
	}
	set systemConnected 1
	if {$correspondingHeaderIncluded == 0} { report $fileName $lineCount "Invalid order of includes" }
	continue
     }
     
    if {![regexp {#include} $line] && !$allIncluded && $lineCount > 1} {
	set allIncluded 1
	set prevLine [getLine $fileName [expr $lineCount - 1]]
	if {![regexp {^$} $prevLine]} { report $fileName $lineCount "No new line after include" }
	incr lineCount
     }
  }
}


foreach fileName [getSourceFileNames] {
  checkIncludeOrder $fileName
}