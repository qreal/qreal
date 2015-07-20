#!/usr/bin/tclsh
# Source files should not use the '\r' (CR) character

proc getFiles { folder dirList } {
  set listOfFiles ""
  # regex won't work if in path will be multiple qreal folders
  set matched 0
  foreach path $dirList {
    if {[string match *$path* $folder] == 1} { 
      if {$path != ""} {
        set matched 1 
      }
    }
  }
  if {$matched == 1} { 
    return ""
  }
  foreach sourceFile [glob -nocomplain -type f -directory $folder *] {
    if [regexp -nocase {.*[.]cpp$|.*[.]h$} $sourceFile ] {
      set listOfFiles [concat $listOfFiles $sourceFile]
    }
  }
  foreach subfolder [glob -nocomplain -type {d r} -directory $folder *] {    
    set listOfFiles [concat $listOfFiles [getFiles $subfolder $dirList]]
  }
  return $listOfFiles
  
}

set fp [open "buildScripts/vera++/exclude" r]
set file_data [read $fp]
close $fp
set dirList [split $file_data "\n"]
set parameters ""
set parameters [concat $parameters [getFiles [pwd] $dirList]]
set fo [open "buildScripts/vera++/params" "w" ]
foreach par $parameters {
  puts $fo $par
}
close $fo
