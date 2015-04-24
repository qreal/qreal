#! /usr/bin/tclsh

# Copyright 2013-2014 Vladimir Nazarenko and Cybertech Labs Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
      
      