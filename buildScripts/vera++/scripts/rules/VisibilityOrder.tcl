#! /usr/bin/tclsh

# 1 if first preceeds(e.g. order public private returns 1) 0 otherwise
proc order { first second } {
  switch -regexp $first {
    public* {if {[regexp {public*} $second]} { return 0 } else { return 1 } }
    protected* {if {[regexp {private*} $second]} { return 1 } else { return 0 } }
    private* { return 0 }
  }
}

proc checkOrder { fileName } {
  if {![regexp -nocase {.*.h$} $fileName] } { return }
  # whether public methods exceeded
  set currentState "public"
  set cSSlots "public"
  set csSign "public"
  set lineCount 1
  foreach line [getAllLines $fileName] {
    set accessModifier ""
    if {[regexp {(public|private|protected)( (signal|slot):|:)} $line accessModifier]} {
      switch -regexp accessModifier {
	signal:$ {
	  if {[order $accessModifier $cSSign]} {
	    report $fileName $lineCount "Invalid order of modifiers" 
	  } else { 
	    regexp {(public|private|protected)} $accessModifier cSSign
	  }
	}
	slot:$ {
	  if {[order $accessModifier $cSSlots]} {
	    report $fileName $lineCount "Invalid order of modifiers" 
	  } else { 
	    regexp {(public|private|protected)} $accessModifier cSSlots
	  }
	}
	default { 
	  if {[order $accessModifier $currentState]} {
	    report $fileName $lineCount "Invalid order of modifiers" 
	  } else { 
	    regexp {(public|private|protected)} $accessModifier currentState
	  }
	}
      }
    }
    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  checkOrder $fileName
}
      
      