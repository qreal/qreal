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

proc checkComments { fileName } {
  set lineCount 1
  foreach line [getAllLines $fileName] { 
    if {[regexp {^[ \t]*//.+} $line]} {
      # check language
      if {[regexp {^[ \t]*//.*[а-яА-Я].*} $line]} {
	report $fileName $lineCount "Comments should be written in english"
      }

      # check spaces
      if {![regexp -nocase {^[ \t]*///?(\s{1,}[^\t^ ].*)?$} $line ]} {
	report $fileName $lineCount "Comment text shall be separated by one space from comment symbol"
      }
    }

    incr lineCount
  }
}

foreach fileName [getSourceFileNames] {
  checkComments $fileName
}