# Copyright 2015 Cybertech Labs Ltd.
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

proc CheckOwnership { fileName } {
  if {![regexp -nocase {.*.h$} $fileName] } { return }
  set lineCount 1
  set previousComment ""
  foreach line [getAllLines $fileName] {
    if { [regexp {.*\* [\w]*;} $line] } {
      report $fileName $lineCount "'*' shall be attached to identifier, not type"
    }

    if { [regexp {.* \*[\w]*;} $line] } {
      if { ![regexp {ownership} [string tolower $line] ] &&  ![regexp {ownership} [string tolower $previousComment] ] } {
        report $fileName $lineCount "Raw pointers must have a comment with ownership information - does object have ownership over pointed object or not (so will it delete pointed object itself or not)"
      }
    }

    if { [regexp {///} $line] } {
      set previousComment [concat  $previousComment $line]
    } else {
      set previousComment ""
    }

    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  CheckOwnership $fileName
}