proc checkIncludeOrder { fileName } {
  set parentH 0
  set systemConnected 0
  set fileShortName ""
  regexp {^[^.]*} [file tail $fileName] fileShortName
  if {[regexp -nocase {.*.h$} $fileName] } { set parentH 1 }
  set lineCount 1
  foreach line [getAllLines $fileName] {
    if [regexp [subst -nocommands -nobackslashes {[ \t]*#include[ \t]*\"$fileShortName.h\"[\t ]*}] $line] {
      incr parentH
      if {$parentH == 1} { 
      incr lineCount
      set nextLine [getLine $fileName [expr $lineCount + 1]]
      if {![regexp {^$} $nextLine]} {
	report $fileName $[expr $lineCount + 1] "No new line after include"
      }
      continue 
      }
     } 
     if [regexp {[\t ]*#include[\t ]*<[^>]*>[ \t]*$} $line] {
	if {$parentH == 0} { report $fileName $lineCount "Invalid order of includes" }
	if {$systemConnected == 1} {report $fileName $lineCount "Invalid order of include" }
	incr lineCount
	continue
     }
     if [regexp {[\t ]*#include[\t ]*\"[^\"]*\"[ \t]*$} $line] {
	if {!$systemConnected} {
	  set nextLine [getLine $fileName [expr $lineCount + 1]]
	  if {![regexp {^$} $nextLine]} {
	    eport $fileName $[expr $lineCount + 1] "No new line after include"
	  }
	}
	set systemConnected 1
	if {$parentH == 0} { report $fileName $lineCount "Invalid order of includes" }
	continue
     }
     if {![regexp {^$} $line]} {
      set prevLine [getLine $fileName [expr $lineCount - 1]]
      if {![regexp {^$} $prevLine]} { report $fileName $lineCount "No new line after include"
     incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  checkIncludeOrder $fileName
}