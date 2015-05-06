#!/usr/bin/tclsh

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

# Checks if there is pragma once and there is no ifdef

proc checkGuards { fileName } {
  if {![regexp -nocase {.*.h$} $fileName] } { return }
  set lineCount 1 
  set guardFound 0
  foreach line [getAllLines $fileName] {
    if {[regexp {^[\t ]*#pragma once[\t\r\n ]*} $line] } {
      set guardFound 1 
    }
    if {[regexp {^[\t ]*#ifn?def .*} $line] } {
      set nextLine [getLine $fileName [expr $lineCount + 1]]
      if {[regexp {^[\t ]*#define .*} $nextLine]} {
	set guardFound 1
	report $fileName $lineCount "Old-style include guard"
      }
    }
    incr lineCount
  }
  if {![expr $guardFound]} {
    report $fileName 1 "No include guard in header file"
  }
}

foreach fileName [getSourceFileNames] {
  checkGuards $fileName
}
