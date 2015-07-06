#!/usr/bin/tclsh

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

# Checks if there is pragma once and there is no ifdef

proc checkCopyrightNotice { fileName } {
  set firstLine  [lindex [getAllLines $fileName] 0]
  set lineCount 1 
  if {![regexp {^/\* Copyright} $firstLine] } {
    report $fileName 1 "No copyright notice" 
  }
}

foreach fileName [getSourceFileNames] {
  checkCopyrightNotice $fileName
}
