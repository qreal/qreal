#! /usr/bin/tclsh

set fp [open "log" r]
set file_data [read $fp]
close $fp
set dirList [split $file_data "\n"]
if {$dirList != ""} {
  foreach l $dirList {
    puts $l
  }
  exit 42 
} else {
  exit 0
}