proc CheckCommaAtTheEnd { fileName } {
  set lineCount 1
  foreach line [getAllLines $fileName] {
    if { [regexp -nocase {.*,[ \t]*$} $line] && ![regexp {//} $line]} {
      report $fileName $lineCount "Comma at the end of line"
    }
    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  CheckCommaAtTheEnd $fileName
}