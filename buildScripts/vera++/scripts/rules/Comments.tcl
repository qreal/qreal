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

proc ClassSection { old line } {
  if { [regexp {public:} $line] || [regexp {public slots:} $line] || [regexp {signals:} $line] } {
    return "public"
  }

  if { [regexp {protected:} $line] || [regexp {protected slots:} $line] } {
    return "protected"
  }

  if { [regexp {private:} $line] || [regexp {private slots:} $line] } {
    return "private"
  }

  return $old
}

proc MayBeMethodDeclaration { line inClass } {
  if { ![regexp { \w+\(([\w:]+ [*& ]*\w+.*)?\)} $line] && ![regexp {\t\w+\([\w:]+ [*& ]*\w+.*\)} $line]} {
    # Regular method (space before name, name, optional parameters) or constructor (tab before name, name, parameters, since parameterless constructors may be not documented)
    return 0
  }

  if { $inClass == 0 } {
    return 0
  }

  return 1
}

proc ShallHaveComment { line previousComment currentSection inClass } {
  if { [string length $previousComment] > 0 } {
    return 0
  }

  if { [string compare $currentSection "private"] == 0 || [string compare $currentSection "protected"] == 0 } {
    return 0
  }  

  if { [regexp {override} $line] } {
    return 0
  }

  if { [regexp {^[\t]*///} $line] } {
    return 0
  }

  return 1
}

proc CheckComments { fileName } {
  if {![regexp -nocase {.*.h$} $fileName] } { return }
  set lineCount 1
  set previousComment ""
  set currentSection ""
  set inClass 0
  foreach line [getAllLines $fileName] {
    set currentSection [ClassSection $currentSection $line]
    if { [regexp {^\t*class[^;]*$} $line] } {
      set inClass 1
      if { [string length $previousComment] == 0} {
        report $fileName $lineCount "Classes must have comments in doxygen style (starting with ///)"
      }
    }

    if { [MayBeMethodDeclaration $line $inClass] && [ShallHaveComment $line $previousComment $currentSection $inClass ] } {
      report $fileName $lineCount "Methods must have comments in doxygen style (starting with ///)"
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
  CheckComments $fileName
}