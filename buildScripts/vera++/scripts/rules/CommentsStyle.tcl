#! /usr/bin/tclsh

proc checkComments { fileName } {
  set lineCount 1
  foreach line [getAllLines $fileName] { 
    if {[regexp {^[ \t]*//.+} $line]} {
      # check lang
      if {[regexp {^[ \t]*//.*[а-яА-Я].*} $line]} {
	report $fileName $lineCount "Comments should be written in english"
      }
      # check spaces
      if {![regexp -nocase {^[ \t]*///?\s{1,}[^\t^ ].*} $line ]} {
	report $fileName $lineCount "Invalid comment style"
      }
    }
    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  checkComments $fileName
}