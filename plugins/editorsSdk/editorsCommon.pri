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

QT += xml widgets
TEMPLATE = lib
CONFIG += plugin

include($$PWD/../../global.pri)

isEmpty(ROOT) {
	error(Please set ROOT variable in a .pro file of your editor as a path to a root folder of QReal sources)
}

win32 {
	QRXC = $$DESTDIR/qrxc$${CONFIGURATION_SUFFIX}.exe
}

unix:!macx {
	QRXC = $$DESTDIR/qrxc$$CONFIGURATION_SUFFIX
}

macx {
	QRXC = $$DESTDIR/qrxc$${CONFIGURATION_SUFFIX}.app/Contents/MacOS/qrxc$$CONFIGURATION_SUFFIX
}

QRXC_DIR = $$DESTDIR
DESTDIR = $$DESTDIR/plugins/editors/

# Needed because somehow QMake fails to create .rcc directory if RESOURCES is in extra compiler output.
RCC_DIR = .build/$$CONFIGURATION/obj

links(qrkernel)

isEmpty(QREAL_EDITOR_PATH) {
	error(Please set QREAL_EDITOR_PATH variable in a .pro file of your editor as a path to that editor .xml file relative to plugins/)
}

if (equals(QMAKE_CXX, "g++")) {
	QMAKE_LFLAGS += -Wl,-E
}

include(extraCompilers.pri)

include(editorsSdk.pri)
