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

TARGET = qrgui_unittests

include(../common.pri)

links(qrkernel qslog qrutils qrrepo qrgui-controller qrgui-plugin-manager qrgui-tool-plugin-interface)

INCLUDEPATH += \
	# A little hack to make .ui files happy. They include other files by relative path based on qrgui/.ui \
	$$PWD/../../../qrgui/icons \
	$$PWD/../../../qrgui/ \

include(modelsTests/modelsTests.pri)

include(helpers/helpers.pri)
