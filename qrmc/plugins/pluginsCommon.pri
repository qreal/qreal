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

include(../../global.pri)

DESTDIR = $$DESTDIR/plugins/editors

TEMPLATE = lib
CONFIG += plugin

isEmpty(QREAL_EDITOR_NAME) {
	error(Please set QREAL_EDITOR_NAME variable in a .pro file of your editor as a folder name of that editor)
}

if (equals(QMAKE_CXX, "g++")) {
	QMAKE_LFLAGS += -Wl,-E
}

QRMC_ROOT = ..

include(pluginsSdk.pri)

includes(qrgui)