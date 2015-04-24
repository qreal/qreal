# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

QREAL_XML = refactoringEditor.xml

QREAL_EDITOR_PATH = refactoring/editor
ROOT = ../../..

include (../../editorsSdk/editorsCommon.pri)

win32 {
    QMAKE_POST_LINK = "xcopy images ..\\..\\..\\bin\\images /s /e /q /y /i"
}
else {
    QMAKE_POST_LINK = "mkdir ../../../bin/images/; cp -r images/* ../../../bin/images/"
}



