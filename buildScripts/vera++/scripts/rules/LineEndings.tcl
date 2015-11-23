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

# Check that a file has platform-specific line endings.

if {$tcl_platform(platform) == "windows"} {
    set eol "\r\n"
} elseif {$tcl_platform(platform) == "unix"} {
    set eol "\n"
} else {
    set eol "\r"
}

foreach fileName [getSourceFileNames] {
    set lineNumber 1
  
    set fd [open $fileName r]
    fconfigure $fd -translation binary
    set data [read $fd]
    close $fd

    # Some hack to split string on another string --- replacing EOL string by some char, which is 
    # unlikely to occur in a code, then splitting on that char
    set lines [split [regsub -all $eol $data "\001"] "\001"]

    foreach line $lines {
        if {[regexp {\r} $line] || [regexp {\n} $line] || [regexp {\r\n} $line]}  {
            report $fileName $lineNumber "Incorrect line ending detected, check your git settings (Auto CrLf must be true on Windows, or checkout platform-specific, commit linux-style)"
            report $fileName $lineNumber "Use 'git config --global core.autocrlf true' or 'git config --global core.autocrlf input' command or equivalent in your favorite git client"
            break
        }

        incr lineNumber
    }
}
