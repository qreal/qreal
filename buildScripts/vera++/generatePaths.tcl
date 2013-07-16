#!/usr/bin/tclsh
# Source files should not use the '\r' (CR) character

proc getFiles { folder } {
  set listOfFiles ""
  foreach sourceFile [glob -nocomplain -type f -directory $folder *] {
    if [regexp -nocase {.*[.]cpp$|.*[.]h$} $sourceFile ] {
      set listOfFiles [concat $listOfFiles $sourceFile]
    }
  }
  foreach subfolder [glob -nocomplain -type {d r} -directory $folder *] {    
    set listOfFiles [concat $listOfFiles [getFiles $subfolder ]]
  }
  return $listOfFiles
}

set parameters ""
set parameters [concat $parameters [getFiles [pwd]]]
set fo [open "buildScripts/vera++/params" "w" ]
puts $fo $parameters
close $fo